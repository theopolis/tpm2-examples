## TPM2 Examples

Several TPM2 TSS API examples.

### System API (SAPI)

Most of this code provide example client interfaces for the TSS 1.0 specification of the system API.

This uses the [TPM2.0-TSS](https://github.com/01org/TPM2.0-TSS) project's tpm2sapi library.

### Build

This assumes a Ubuntu 14.04 install, defer to installing the package equivilents for your distribution otherwise.

Install the TPM2.0-TSS dependencies:
```
# sudo apt-get install autoconf libtool
# audo apt-get install autoconf-archive # Need AX_PTHREADS macro
```

Build and install the TPM2SAPI library:
```
# git clone https://github.com/theopolis/TPM2.0-TSS
# pushd TPM2.0-TSS
# ./bootstrap
# mkdir build; cd build
# ../configure
# make; sudo make install
# popd
```

# Build this code:
```
# make
```

