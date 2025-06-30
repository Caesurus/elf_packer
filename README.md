# Encrypted ELF loader

A small elf loader. It can load static and dynamically linked ELF EXEC and DYN (pie) binaries. The loader is PIE program that doesn't depend on libc and calls kernel services directly (z_syscall.c).

If the loader needs to load a dynamically linked ELF it places an interpreter (usually ld.so) and a requested binary into a memory and then calls the interpreter entry point.

This branch adds the aes implementation from: [tiny-AES-c](https://github.com/kokke/tiny-AES-c/)

The IV will be generated on each build, so no two binaries will have the same IV.

The `payload.elf` is encrypted and output to `payload.enc`, which is then used to create a header file to be compiled into the final loader.

At runtime it will read out the user environment for `AES_KEY` and decrypt the ELF file in memory, load it, and execute it.

## Build

Default build is for amd64:

```
$ ./make_example_payload_elf.sh
$ make
$ AES_KEY=00112233445566778899aabbccddeeff ./loader
```

Build for i386:

```
$ M32=1 ./make_example_payload_elf.sh
$ make ARCH=i386
$ AES_KEY=00112233445566778899aabbccddeeff ./loader
```

Small build (exclude all messages and printf):

```
$ make SMALL=1
```
