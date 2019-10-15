#include <DFUAsyncWorker.h>

extern "C" {
    #include "project.h"
    #include "dfu.h"
}

DFUAsyncWorker::DFUAsyncWorker(std::string &bdAddr,
                               std::string &zipPath,
                               Function &callback) : AsyncWorker(callback),
                                    bdAddr(bdAddr),
                                    zipPath(zipPath),
                                    result(-2)
{
}

void DFUAsyncWorker::Execute()
{
    uint8_t *bin;
    size_t bin_size;
    uint8_t *dat;
    size_t dat_size;
    struct zip *zip;
    char *m_str;
    struct manifest *m;

    ble_dfu_resp_val_t res = BLE_DFU_NO_OPERATION;

    zip = open_zip (zipPath.c_str());
    read_file_from_zip(zip, "manifest.json", &m_str);
    m = parse_manifest (m_str);
    if (m == NULL) {
        res = BLE_DFU_INVALID_MANIFEST;
        this->result = res;
        return;
    }

    if (m->hasSDBootloader) {
        dat_size = read_file_from_zip(zip, m->sdBootloaderDatFileName, &dat);
        bin_size = read_file_from_zip(zip, m->sdBootloaderBinFileName, &bin);
        
        printf ("%u bytes init_data, %u bytes bootloader\n\n", (unsigned) dat_size, (unsigned) bin_size);
        res = dfu(bdAddr.c_str(), dat, dat_size, bin, bin_size);
        if (res != BLE_DFU_RESP_VAL_SUCCESS) {
            this->result = res;
            return;
        }
        sleep(5);
    }

    if (m->hasInternal) {
        dat_size = read_file_from_zip(zip, m->internalDatFileName, &dat);
        bin_size = read_file_from_zip(zip, m->internalBinFileName, &bin);
        
        printf ("%u bytes init_data, %u bytes internal\n\n", (unsigned) dat_size, (unsigned) bin_size);
        res = dfu(bdAddr.c_str(), dat, dat_size, bin, bin_size);
        if (res != BLE_DFU_RESP_VAL_SUCCESS) {
            this->result = res;
            return;
        }
        sleep(5);
    }

    if (m->hasApplication) {
        dat_size = read_file_from_zip(zip, m->applicationDatFileName, &dat);
        bin_size = read_file_from_zip(zip, m->applicationBinFileName, &bin);
        
        printf ("%u bytes init_data, %u bytes firmware\n\n", (unsigned) dat_size, (unsigned) bin_size);

        res = dfu(bdAddr.c_str(), dat, dat_size, bin, bin_size);
    }
    this->result = res;
}

void DFUAsyncWorker::OnOK()
{
    HandleScope scope(Env());
    Callback().Call({Number::New(Env(), this->result)});
}