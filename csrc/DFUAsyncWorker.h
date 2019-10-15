#include <napi.h>

using namespace Napi;

class DFUAsyncWorker : public AsyncWorker
{
    public:
        DFUAsyncWorker(std::string &bdAddr, std::string &zipPath, Function &callback);

        void Execute();

        void OnOK();
    private:
        std::string bdAddr;
        std::string zipPath;
        int32_t result;
};