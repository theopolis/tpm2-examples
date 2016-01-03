/*
 * TPM2 Examples
 * Copyright (C) 2006 devnull
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 */

#include <string>
#include <iomanip>
#include <iostream>

#include "tpm2.h"

// Todo: refactor out a global sys context.
TSS2_SYS_CONTEXT *sysContext;

// Todo: still trying to understand tpm2tlci-related libs.
TSS2_RC PlatformCommand(
    TSS2_TCTI_CONTEXT *tctiContext,     /* in */
    char cmd ) { return 0; }
FILE *outFp;

void tpm2_readpcrs(size_t k) {
  TPML_PCR_SELECTION pcrSelection;

  pcrSelection.count = 1;
  pcrSelection.pcrSelections[0].hash = TPM_ALG_SHA1;
  pcrSelection.pcrSelections[0].sizeofSelect = 3;

  // Clear out PCR select bit field
  pcrSelection.pcrSelections[0].pcrSelect[0] = 0;
  pcrSelection.pcrSelections[0].pcrSelect[1] = 0;
  pcrSelection.pcrSelections[0].pcrSelect[2] = 0;

  // Now set the PCR you want to read
  SET_PCR_SELECT_BIT(pcrSelection.pcrSelections[0], k);

  uint32_t pcrUpdateCounterBeforeExtend;
  TPML_PCR_SELECTION pcrSelectionOut;
  TPML_DIGEST pcrValues;

  TSS2_RC res = Tss2_Sys_PCR_Read(sysContext,
                    0,
                    &pcrSelection,
                    &pcrUpdateCounterBeforeExtend,
                    &pcrSelectionOut,
                    &pcrValues,
                    0);

  if (res == TSS2_RC_SUCCESS) {
    size_t digest_size = pcrValues.digests[0].t.size;
    std::cout << "PCR " << k << ": 0x";
    for (size_t i = 0; i < digest_size; i++) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (int)pcrValues.digests[0].t.buffer[i];
    }
    std::cout << std::endl;
  }
}

TSS2_TCTI_DRIVER_INFO resMgrInterfaceInfo = {
    "resMgr", "", InitSocketsTcti, TeardownSocketsTcti};

TSS2_RC InitTctiResMgrContext(const char *config,
                              const char *name,
                              TSS2_TCTI_CONTEXT **context) {
  size_t size;

  TSS2_RC result;
  result = resMgrInterfaceInfo.initialize(NULL, &size, config, 0, 0, name, 0);
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

TSS2_SYS_CONTEXT *InitSysContext(UINT16 maxCommandSize,
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

int main(int argc, char *argv[]) {
  std::string config = "127.0.0.1 " + std::to_string(DEFAULT_RESMGR_TPM_PORT) + " ";
  std::string interface = "Resource Manager";
  TSS2_ABI_VERSION abi_version = {
      TSSWG_INTEROP,
      TSS_SAPI_FIRST_FAMILY,
      TSS_SAPI_FIRST_LEVEL,
      TSS_SAPI_FIRST_VERSION,
  };

  TSS2_TCTI_CONTEXT *context = nullptr;
  InitTctiResMgrContext(config.c_str(), interface.c_str(), &context);
  sysContext = InitSysContext(0, context, &abi_version);

  // Read PCRs 0 - 17
  for (size_t k = 0; k < 18; k++) {
    tpm2_readpcrs(k);
  }

  return 0;
}
