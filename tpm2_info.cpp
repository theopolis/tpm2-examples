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

#include <string>
#include <iomanip>
#include <iostream>

#include "tpm2.h"

#define MSFT_MANUFACTURER_ID 0x4d534654

void tpm2_info(TSS2_SYS_CONTEXT* context) {
  TSS2_RC rval = TSS2_RC_SUCCESS;
  TPMS_CAPABILITY_DATA capabilityData;

  rval = Tss2_Sys_GetCapability(context, 0, TPM_CAP_TPM_PROPERTIES,
                                TPM_PT_REVISION, 1, 0, &capabilityData, 0);

  UINT32 tpmSpecVersion = 0;

  if (capabilityData.data.tpmProperties.count == 1 &&
      (capabilityData.data.tpmProperties.tpmProperty[0].property ==
       TPM_PT_REVISION)) {
    tpmSpecVersion = capabilityData.data.tpmProperties.tpmProperty[0].value;
  }

  std::cout << "TPM SPEC Version: 0x" << std::hex << std::setw(4)
            << std::setfill('0') << tpmSpecVersion << std::endl;

  UINT32 tpmManufacturer = 0;

  rval = Tss2_Sys_GetCapability(context, 0, TPM_CAP_TPM_PROPERTIES,
                                TPM_PT_MANUFACTURER, 1, 0, &capabilityData, 0);

  if (capabilityData.data.tpmProperties.count == 1 &&
      (capabilityData.data.tpmProperties.tpmProperty[0].property ==
       TPM_PT_MANUFACTURER)) {
    tpmManufacturer = capabilityData.data.tpmProperties.tpmProperty[0].value;
  }

  std::cout << "TPM Manufacturer: 0x" << std::hex << std::setw(8)
            << std::setfill('0') << tpmManufacturer << std::endl;
  std::cout << "TPM Manufacturer Name: ";
  for (int i = 3; i >= 0; i--) {
    std::cout << (char)((tpmManufacturer >> (i * 8)) & (0xFF));
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {

  auto mgr = ContextManager::instance();
  if (!mgr->open("127.0.0.1", DEFAULT_RESMGR_TPM_PORT)) {
    return 1;
  }

  tpm2_info(mgr->getContext());
  return 0;
}
