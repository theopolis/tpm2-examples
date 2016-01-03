
#include <stdint.h>

// Interesting use of defines within TPM2.0-TSS
#define linux

#include <tpm2sapi/tpm20.h>

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


#include <tpm2tcti/tpmsockets.h>
#include <tpm2sapi/tss2_sysapi_util.h>

#define TCTI_MAGIC 0x7e18e9defa8bc9e2
#define TCTI_VERSION 0x1

#if !defined(DEFAULT_RESMGR_TPM_PORT)
#define DEFAULT_RESMGR_TPM_PORT 2323
#endif

#define HOSTNAME_LENGTH 200
#define PORT_LENGTH 4
