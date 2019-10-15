#define NAPI_VERSION 3
#include <napi.h>
#include "DFUAsyncWorker.h"

extern "C" {
    #include "project.h"
    #include "dfu.h"
}

using namespace Napi;

namespace nrfdfu
{
    Value uploadFirmware(const Napi::CallbackInfo& info)
    {
        Env env = info.Env();

        if (info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsFunction()) {
            TypeError::New(env, "Two strings as arguments expected")
                .ThrowAsJavaScriptException();
        }

        std::string bdAddr = info[0].As<String>();
        std::string zipPath = info[1].As<String>();
        Function callback = info[2].As<Function>();

        DFUAsyncWorker *worker = new DFUAsyncWorker(bdAddr, zipPath, callback);
        worker->Queue();

        return info.Env().Undefined();
    }

    Object Init(Env env, Object exports)
    {
        exports.Set("uploadFirmware", Function::New(env, nrfdfu::uploadFirmware));
        return exports;
    }

    NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace nrfdfu
