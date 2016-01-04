/*
 * TPM2 Examples
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 */

#include "tpm2.h"

extern "C" {
// Todo: refactor out a global sys context.
TSS2_SYS_CONTEXT *sysContext;

// Todo: still trying to understand tpm2tlci-related libs.
TSS2_RC PlatformCommand(TSS2_TCTI_CONTEXT *, char) { return 0; }
int TpmClientPrintf(UINT8, const char *, ...) { return 0; }
FILE *outFp;
}

TSS2_RC initTctiResMgrContext(const char *config,
                              const char *name,
                              TSS2_TCTI_CONTEXT **context) {
  static TSS2_TCTI_DRIVER_INFO resMgrInterfaceInfo = {
      "resMgr", "", InitSocketsTcti, TeardownSocketsTcti};

  size_t size;

  auto result =
      resMgrInterfaceInfo.initialize(NULL, &size, config, 0, 0, name, 0);
  if (result != TSS2_RC_SUCCESS) {
    return result;
  }

  *context = (TSS2_TCTI_CONTEXT *)malloc(size);

  if (*context != nullptr) {
    result = resMgrInterfaceInfo.initialize(
        *context, &size, config, TCTI_MAGIC, TCTI_VERSION, name, 0);
  } else {
    result = TSS2_TCTI_RC_BAD_CONTEXT;
  }

  return result;
}

TSS2_SYS_CONTEXT *initSysContext(UINT16 maxCommandSize,
                                 TSS2_TCTI_CONTEXT *tctiContext,
                                 TSS2_ABI_VERSION *abiVersion) {
  UINT32 contextSize;
  TSS2_RC rval;
  TSS2_SYS_CONTEXT *sysContext;

  // Get the size needed for system context structure.
  contextSize = Tss2_Sys_GetContextSize(maxCommandSize);

  // Allocate the space for the system context structure.
  sysContext = (TSS2_SYS_CONTEXT *)malloc(contextSize);

  if (sysContext != nullptr) {
    // Initialized the system context structure.
    rval =
        Tss2_Sys_Initialize(sysContext, contextSize, tctiContext, abiVersion);

    if (rval == TSS2_RC_SUCCESS) {
      return sysContext;
    } else {
      return nullptr;
    }
  }

  return nullptr;
}

bool ContextManager::open(const std::string &host, size_t port) {
  if (context != nullptr) {
    return false;
  }

  std::string config = host + " " + std::to_string(port) + " ";
  std::string interface = "Resource Manager";
  TSS2_ABI_VERSION abi_version = {
      TSSWG_INTEROP,
      TSS_SAPI_FIRST_FAMILY,
      TSS_SAPI_FIRST_LEVEL,
      TSS_SAPI_FIRST_VERSION,
  };

  initTctiResMgrContext(config.c_str(), interface.c_str(), &context);
  system_context = initSysContext(0, context, &abi_version);
  return (system_context != nullptr);
}
