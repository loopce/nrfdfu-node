struct manifest
{
  char hasApplication;
  const char *applicationDatFileName;
  const char *applicationBinFileName;
  char hasSDBootloader;
  const char *sdBootloaderDatFileName;
  const char *sdBootloaderBinFileName;
  char hasInternal;
  const char *internalDatFileName;
  const char *internalBinFileName;
};
