#define NAPI_VERSION 3
#include <napi.h>

extern "C" {
    #include "project.h"
    #include "dfu.h"
}

namespace nrfdfu
{
    Napi::Number uploadFirmware(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
            Napi::TypeError::New(env, "Two strings as arguments expected").ThrowAsJavaScriptException();
        }

        std::string bdAddr = std::string(info[0].As<Napi::String>());
        std::string zipPath = std::string(info[1].As<Napi::String>());

        uint8_t *bin;
        size_t bin_size;
        uint8_t *dat;
        size_t dat_size;
        struct zip *zip;
        char *m_str;
        struct manifest *m;

        ble_dfu_resp_val_t result = BLE_DFU_NO_OPERATION;

        zip = open_zip (zipPath.c_str());
        read_file_from_zip(zip, "manifest.json", &m_str);
        m = parse_manifest (m_str);

        if (m->hasSDBootloader) {
            dat_size = read_file_from_zip(zip, m->sdBootloaderDatFileName, &dat);
            bin_size = read_file_from_zip(zip, m->sdBootloaderBinFileName, &bin);
            
            printf ("%u bytes init_data, %u bytes SD+bootloader\n\n", (unsigned) dat_size, (unsigned) bin_size);
            result = dfu(bdAddr.c_str(), dat, dat_size, bin, bin_size);
            if (result != BLE_DFU_RESP_VAL_SUCCESS) {
                return Napi::Number::New(env, result);
            }
            sleep(5);
        }

        if (m->hasApplication) {
            dat_size = read_file_from_zip(zip, m->applicationDatFileName, &dat);
            bin_size = read_file_from_zip(zip, m->applicationBinFileName, &bin);
            
            printf ("%u bytes init_data, %u bytes firmware\n\n", (unsigned) dat_size, (unsigned) bin_size);

            result = dfu(bdAddr.c_str(), dat, dat_size, bin, bin_size);
        }
        return Napi::Number::New(env, result);
    }

    Napi::Object Init(Napi::Env env, Napi::Object exports)
    {
        exports.Set("uploadFirmware", Napi::Function::New(env, nrfdfu::uploadFirmware));
        return exports;
    }

    NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace nrfdfu
