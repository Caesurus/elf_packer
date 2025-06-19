ELF_ORIG=payload.elf
ELF_ENC=payload.enc
PAYLOAD_HEADER=payload.h
AES_KEY=00112233445566778899aabbccddeeff
IV_BIN := iv.bin

# make ARCH=i386 SMALL=1 DEBUG=1
ARCH ?= amd64

ARCHS32 := i386 arm mips
ARCHS64 := amd64 aarch64
ARCHS := $(ARCHS32) $(ARCHS64)

CFLAGS += -pipe -Wall -Wextra -fPIC -fno-ident -fno-stack-protector -U _FORTIFY_SOURCE -ffreestanding -static
LDFLAGS += -nostartfiles -nodefaultlibs -nostdlib
LDFLAGS += -pie -e z_start -Wl,-Bsymbolic,--no-undefined,--build-id=none -Wl,-Ttext-segment=0x66600000
TARGET := loader

# check for xxd
XXD := $(shell command -v xxd)
ifeq ($(XXD),)
  $(error xxd is required but not installed)
endif

ifeq "$(filter $(ARCH),$(ARCHS))" ""
  $(error ARCH='$(ARCH)' is not supported)
endif

ifeq ($(ARCH),i386)
  CFLAGS += -m32
  ASFLAGS += -m32
  LDFLAGS += -m32
endif

ifeq "$(filter $(ARCH),$(ARCHS32))" "$(ARCH)"
  CFLAGS += -DELFCLASS=ELFCLASS32
else
  CFLAGS += -DELFCLASS=ELFCLASS64
endif

ifdef DEBUG
  CFLAGS += -O0 -g
  ASFLAGS += -g
else
# Disable unwind info to make prog smaller.
  CFLAGS += -fvisibility=hidden -fno-asynchronous-unwind-tables -fno-unwind-tables
  #CFLAGS += -Os
  #LDFLAGS += -s
endif

OBJS := $(patsubst %.c,%.o, $(wildcard *.c))
OBJS += $(patsubst %.S,%.o, $(wildcard $(ARCH)/*.S))

ifdef SMALL
  OBJS := $(filter-out z_printf.%,$(OBJS))
  OBJS := $(filter-out z_err.%,$(OBJS))
  CFLAGS += -DZ_SMALL
endif

.PHONY: clean all $(ELF_ENC) $(PAYLOAD_HEADER) $(IV_BIN)

all: $(TARGET)

$(IV_BIN):
	@head -c 16 /dev/urandom > $(IV_BIN)

$(ELF_ENC): $(ELF_ORIG) $(IV_BIN)
	openssl enc -aes-128-cbc -K $(AES_KEY) -iv $(shell xxd -p $(IV_BIN)) -nosalt -in $(ELF_ORIG) -out $(ELF_ENC)
	@truncate -s %16 $(ELF_ENC)

$(PAYLOAD_HEADER): $(ELF_ENC)
	@echo "#ifndef _PAYLOAD_H_"> $(PAYLOAD_HEADER)
	@echo "#define _PAYLOAD_H_">> $(PAYLOAD_HEADER)
	@xxd -i $(ELF_ENC) >> $(PAYLOAD_HEADER)
	@xxd -i $(IV_BIN) >> $(PAYLOAD_HEADER)
	@echo "#endif">> $(PAYLOAD_HEADER)

loader: $(PAYLOAD_HEADER) $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

clean:
	rm -rf *.o $(TARGET) */*.o $(ELF_ENC) $(IV_BIN)
	@echo "#error generate file" > $(PAYLOAD_HEADER)

