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

#include <memory>

#include <stdint.h>

// Interesting use of defines within TPM2.0-TSS
#define linux

#include <tpm2sapi/tpm20.h>
#include <tpm2tcti/tpmsockets.h>
#include <tpm2sapi/tss2_sysapi_util.h>

#define TCTI_MAGIC 0x7e18e9defa8bc9e2
#define TCTI_VERSION 0x1

#if !defined(DEFAULT_RESMGR_TPM_PORT)
#define DEFAULT_RESMGR_TPM_PORT 2323
#endif

#define HOSTNAME_LENGTH 200
#define PORT_LENGTH 4

class ContextManager {
 public:
  using SelfRef = std::shared_ptr<ContextManager>;

  /// Singleton accessor.
  static SelfRef instance() {
    static SelfRef ref = SelfRef(new ContextManager());
    return ref;
  }

  /// Initialize the TCTI and SYS context.
  bool open(const std::string& host, size_t port);

  /// Accessor for SYS context.
  TSS2_SYS_CONTEXT* getContext() { return system_context; }

 public:
  ~ContextManager() {}

 private:
  ContextManager() {}

 private:
  TSS2_SYS_CONTEXT* system_context{nullptr};
  TSS2_TCTI_CONTEXT* context{nullptr};
};
