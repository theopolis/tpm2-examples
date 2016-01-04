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

#define PCR_0 0
#define PCR_1 1
#define PCR_2 2
#define PCR_3 3
#define PCR_4 4
#define PCR_5 5
#define PCR_6 6
#define PCR_7 7
#define PCR_8 8
#define PCR_9 9
#define PCR_10 10
#define PCR_11 11
#define PCR_12 12
#define PCR_13 13
#define PCR_14 14
#define PCR_15 15
#define PCR_16 16
#define PCR_17 17
#define PCR_18 18

#define PCR_SIZE 20

#define SET_PCR_SELECT_BIT(pcrSelection, pcr) \
  (pcrSelection).pcrSelect[((pcr) / 8)] |= (1 << ((pcr) % 8));

void tpm2_readpcrs(TSS2_SYS_CONTEXT* context, size_t k) {
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

  TSS2_RC res = Tss2_Sys_PCR_Read(context,
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

int main(int argc, char* argv[]) {

  auto mgr = ContextManager::instance();
  if (!mgr->open("127.0.0.1", DEFAULT_RESMGR_TPM_PORT)) {
    return 1;
  }

  // Read PCRs 0 - 17
  for (size_t k = 0; k < 18; k++) {
    tpm2_readpcrs(mgr->getContext(), k);
  }

  return 0;
}
