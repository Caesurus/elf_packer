ELF_ORIG=payload.elf
ELF_ENC=payload.enc
PAYLOAD_HEADER=payload.h
AES_IV=aa55aa55aa55aa55aa55aa55aa55aa55
AES_KEY=00112233445566778899aabbccddeeff

# make ARCH=i386 SMALL=1 DEBUG=1
ARCH ?= amd64

ARCHS32 := i386
ARCHS64 := amd64 aarch64
ARCHS := $(ARCHS32) $(ARCHS64)

CFLAGS += -pipe -Wall -Wextra -fPIC -fno-ident -fno-stack-protector -U _FORTIFY_SOURCE -ffreestanding
LDFLAGS += -nostartfiles -nodefaultlibs -nostdlib
LDFLAGS += -pie -e z_start -Wl,-Bsymbolic,--no-undefined,--build-id=none
TARGET := loader

ifeq "$(filter $(ARCH),$(ARCHS))" ""
  $(error ARCH='$(ARCH)' is not supported)
endif

ifeq "$(filter $(ARCH),$(ARCHS32))" "$(ARCH)"
  CFLAGS += -m32 -DELFCLASS=ELFCLASS32
  ASFLAGS += -m32
  LDFLAGS += -m32
else
  CFLAGS += -DELFCLASS=ELFCLASS64
endif

ifdef DEBUG
  CFLAGS += -O0 -g
  ASFLAGS += -g
else
  CFLAGS += -fvisibility=hidden
  # Disable unwind info to make prog smaller.
  CFLAGS += -Os -fno-asynchronous-unwind-tables -fno-unwind-tables
  LDFLAGS += -s
endif

OBJS := $(patsubst %.c,%.o, $(wildcard *.c))
OBJS += $(patsubst %.S,%.o, $(wildcard $(ARCH)/*.S))

ifdef SMALL
  OBJS := $(filter-out z_printf.%,$(OBJS))
  OBJS := $(filter-out z_err.%,$(OBJS))
  CFLAGS += -DZ_SMALL
endif

.PHONY: clean all $(ELF_ENC) $(PAYLOAD_HEADER)

all: $(TARGET)

$(ELF_ENC): $(ELF_ORIG)
	@openssl enc -aes-128-cbc -K $(AES_KEY) -iv $(AES_IV) -nosalt -in $(ELF_ORIG) -out $(ELF_ENC)
	@truncate -s %16 $(ELF_ENC)

$(PAYLOAD_HEADER): $(ELF_ENC)
	@echo "#ifndef _PAYLOAD_H_"> $(PAYLOAD_HEADER)
	@echo "#define _PAYLOAD_H_">> $(PAYLOAD_HEADER)
	@xxd -i $(ELF_ENC) >> $(PAYLOAD_HEADER)
	@echo "#endif">> $(PAYLOAD_HEADER)

loader: $(PAYLOAD_HEADER) $(OBJS)

clean:
	rm -rf *.o $(TARGET) */*.o $(ELF_ENC)
	@echo "#error generate file" > $(PAYLOAD_HEADER)

