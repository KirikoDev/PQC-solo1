include build/common.mk

# ST related
SRC = src/main.c src/init.c src/redirect.c src/flash.c src/rng.c src/led.c src/device.c
SRC += src/fifo.c src/attestation.c src/nfc.c src/ams.c src/sense.c
SRC += src/startup_stm32l432xx.s src/system_stm32l4xx.c
SRC += src/mbedtls_entropy_source.c
SRC += src/mbedtls_time.c
SRC += $(DRIVER_LIBS) $(USB_LIB)

# FIDO2 lib
SRC += ../../fido2/apdu.c ../../fido2/util.c ../../fido2/u2f.c ../../fido2/test_power.c
SRC += ../../fido2/stubs.c ../../fido2/log.c  ../../fido2/ctaphid.c  ../../fido2/ctap.c
SRC += ../../fido2/ctap_parse.c ../../fido2/crypto.c
SRC += ../../fido2/version.c
SRC += ../../fido2/data_migration.c
SRC += ../../fido2/extensions/extensions.c ../../fido2/extensions/solo.c
SRC += ../../fido2/extensions/wallet.c

# Crypto libs
SRC += ../../crypto/sha256/sha256.c ../../crypto/micro-ecc/uECC.c ../../crypto/tiny-AES-c/aes.c
SRC += ../../crypto/cifra/src/sha512.c ../../crypto/cifra/src/blockwise.c
SRC += ../../crypto/kyber/kyber512/aes256ctr.c \
       ../../crypto/kyber/kyber512/cbd.c \
       ../../crypto/kyber/kyber512/fips202.c \
       ../../crypto/kyber/kyber512/indcpa.c \
       ../../crypto/kyber/kyber512/kem.c \
       ../../crypto/kyber/kyber512/ntt.c \
       ../../crypto/kyber/kyber512/poly.c \
       ../../crypto/kyber/kyber512/polyvec.c \
       ../../crypto/kyber/kyber512/reduce.c \
       ../../crypto/kyber/kyber512/rng.c \
       ../../crypto/kyber/kyber512/sha256.c \
       ../../crypto/kyber/kyber512/sha512.c \
       ../../crypto/kyber/kyber512/symmetric-aes.c \
       ../../crypto/kyber/kyber512/symmetric-shake.c \
       ../../crypto/kyber/kyber512/verify.c
#SRC += ../../crypto/kyber/kyber512-90s/aes256ctr.c \
       ../../crypto/kyber/kyber512-90s/cbd.c \
       ../../crypto/kyber/kyber512-90s/fips202.c \
       ../../crypto/kyber/kyber512-90s/indcpa.c \
       ../../crypto/kyber/kyber512-90s/kem.c \
       ../../crypto/kyber/kyber512-90s/ntt.c \
       ../../crypto/kyber/kyber512-90s/poly.c \
       ../../crypto/kyber/kyber512-90s/polyvec.c \
       ../../crypto/kyber/kyber512-90s/reduce.c \
       ../../crypto/kyber/kyber512-90s/rng.c \
       ../../crypto/kyber/kyber512-90s/sha256.c \
       ../../crypto/kyber/kyber512-90s/sha512.c \
       ../../crypto/kyber/kyber512-90s/symmetric-aes.c \
       ../../crypto/kyber/kyber512-90s/symmetric-shake.c \
       ../../crypto/kyber/kyber512-90s/verify.c
#SRC += ../../crypto/kyber/kyber768/aes256ctr.c \
       ../../crypto/kyber/kyber768/cbd.c \
       ../../crypto/kyber/kyber768/fips202.c \
       ../../crypto/kyber/kyber768/indcpa.c \
       ../../crypto/kyber/kyber768/kem.c \
       ../../crypto/kyber/kyber768/ntt.c \
       ../../crypto/kyber/kyber768/poly.c \
       ../../crypto/kyber/kyber768/polyvec.c \
       ../../crypto/kyber/kyber768/reduce.c \
       ../../crypto/kyber/kyber768/rng.c \
       ../../crypto/kyber/kyber768/sha256.c \
       ../../crypto/kyber/kyber768/sha512.c \
       ../../crypto/kyber/kyber768/symmetric-aes.c \
       ../../crypto/kyber/kyber768/symmetric-shake.c \
       ../../crypto/kyber/kyber768/verify.c
#SRC +=  ../../crypto/falcon/falcon512cxm4/codec.c \
		../../crypto/falcon/falcon512cxm4/common.c \
		../../crypto/falcon/falcon512cxm4/fft.c \
		../../crypto/falcon/falcon512cxm4/fpr.c \
		../../crypto/falcon/falcon512cxm4/keygen.c \
		../../crypto/falcon/falcon512cxm4/nist.c \
		../../crypto/falcon/falcon512cxm4/rng.c \
		../../crypto/falcon/falcon512cxm4/shake.c \
		../../crypto/falcon/falcon512cxm4/sign.c \
		../../crypto/falcon/falcon512cxm4/vrfy.c \
		../../crypto/falcon/KAT/generator/katrng.c
#SRC += ../../crypto/dilithium/dilithium2/aes256ctr.c \
       ../../crypto/dilithium/dilithium2/fips202.c \
       ../../crypto/dilithium/dilithium2/sign.c \
       ../../crypto/dilithium/dilithium2/ntt.c \
       ../../crypto/dilithium/dilithium2/packing.c \
       ../../crypto/dilithium/dilithium2/poly.c \
       ../../crypto/dilithium/dilithium2/polyvec.c \
       ../../crypto/dilithium/dilithium2/reduce.c \
       ../../crypto/dilithium/dilithium2/rng.c \
       ../../crypto/dilithium/dilithium2/rounding.c \
       ../../crypto/dilithium/dilithium2/symmetric-aes.c \
       ../../crypto/dilithium/dilithium2/symmetric-shake.c \
       ../../crypto/dilithium/dilithium2/PQCgenKAT_sign.c
#SRC += ../../crypto/dilithium/dilithium2-AES/aes256ctr.c \
       ../../crypto/dilithium/dilithium2-AES/fips202.c \
       ../../crypto/dilithium/dilithium2-AES/sign.c \
       ../../crypto/dilithium/dilithium2-AES/ntt.c \
       ../../crypto/dilithium/dilithium2-AES/packing.c \
       ../../crypto/dilithium/dilithium2-AES/poly.c \
       ../../crypto/dilithium/dilithium2-AES/polyvec.c \
       ../../crypto/dilithium/dilithium2-AES/reduce.c \
       ../../crypto/dilithium/dilithium2-AES/rng.c \
       ../../crypto/dilithium/dilithium2-AES/rounding.c \
       ../../crypto/dilithium/dilithium2-AES/symmetric-aes.c \
       ../../crypto/dilithium/dilithium2-AES/symmetric-shake.c \
       ../../crypto/dilithium/dilithium2-AES/PQCgenKAT_sign.c
#SRC += ../../crypto/sphincs-sha256-128f-simple/address.c \
       ../../crypto/sphincs-sha256-128f-simple/fors.c \
       ../../crypto/sphincs-sha256-128f-simple/hash_sha256.c \
       ../../crypto/sphincs-sha256-128f-simple/sha256.c \
       ../../crypto/sphincs-sha256-128f-simple/PQCgenKAT_sign.c \
       ../../crypto/sphincs-sha256-128f-simple/rng.c \
       ../../crypto/sphincs-sha256-128f-simple/sign.c \
       ../../crypto/sphincs-sha256-128f-simple/thash_sha256_simple.c \
       ../../crypto/sphincs-sha256-128f-simple/utils.c \
       ../../crypto/sphincs-sha256-128f-simple/wots.c
SRC += ../../mbedtls/library/aes.c \
       ../../mbedtls/library/entropy.c \
	 ../../mbedtls/library/platform_util.c \
	 ../../mbedtls/library/md.c \
	 ../../mbedtls/library/sha1.c \
	 ../../mbedtls/library/sha3.c \
	 ../../mbedtls/library/sha256.c \
	 ../../mbedtls/library/sha512.c \
	 ../../mbedtls/library/ripemd160.c \
	 ../../mbedtls/library/md5.c \
       ../../mbedtls/library/ctr_drbg.c


OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.s=.o)

INC = -Isrc/ -Isrc/cmsis/ -Ilib/ -Ilib/usbd/

INC += -I../../fido2/ -I../../fido2/extensions
INC += -I../../tinycbor/src -I../../crypto/sha256 -I../../crypto/micro-ecc
INC += -I../../crypto/tiny-AES-c
INC += -I../../crypto/cifra/src -I../../crypto/cifra/src/ext
INC += -I../../crypto/salty/c-api
INC += -I../../mbedtls/include

SEARCH=-L../../tinycbor/lib -L../../crypto/salty/c-api

ifndef LDSCRIPT
LDSCRIPT=linker/stm32l4xx.ld
endif

CFLAGS= $(INC)

TARGET=solo
HW=-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
 
# Solo or Nucleo board
CHIP=STM32L432xx

ifndef DEBUG
DEBUG=0
endif

DEFINES = -DDEBUG_LEVEL=$(DEBUG) -D$(CHIP) -DAES256=1  -DUSE_FULL_LL_DRIVER -DAPP_CONFIG=\"app.h\" $(EXTRA_DEFINES)

CFLAGS=$(INC) -c $(DEFINES) -I../../mbedtls/include -I../../mbedtls/library -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fdata-sections -ffunction-sections \
	-fomit-frame-pointer $(HW) -g $(VERSION_FLAGS)
CFLAGS += -I../../crypto/kyber/kyber512/
#CFLAGS += -I../../crypto/kyber/kyber512-90s/
#CFLAGS += -I../../crypto/kyber/kyber768/
#CFLAGS += -I../../crypto/falcon/falcon512cxm4/
#CFLAGS += -I../../crypto/dilithium/dilithium2/
#CFLAGS += -I../../crypto/dilithium/dilithium2-AES/
#CFLAGS += -I../../crypto/sphincs-sha256-128f-simple/
LDFLAGS_LIB=$(HW) $(SEARCH) -specs=nano.specs  -specs=nosys.specs  -Wl,--gc-sections -lnosys
LDFLAGS=$(HW) $(LDFLAGS_LIB) -T$(LDSCRIPT) -Wl,-Map=$(TARGET).map,--cref -Wl,-Bstatic  -ltinycbor -lsalty
LDFLAGS += -L../../mbedtls/library -lmbedtls
ECC_CFLAGS = $(CFLAGS) -DuECC_PLATFORM=5 -DuECC_OPTIMIZATION_LEVEL=4 -DuECC_SQUARE_FUNC=1 -DuECC_SUPPORT_COMPRESSED_POINT=0


.PRECIOUS: %.o

all: $(TARGET).elf
	$(SZ) $^

%.o: %.c
	$(CC) $^ $(HW)  -Os $(CFLAGS) -o $@

../../crypto/micro-ecc/uECC.o: ../../crypto/micro-ecc/uECC.c
	$(CC) $^ $(HW)  -O3 $(ECC_CFLAGS) -o $@

%.elf: $(OBJ)
	$(CC) $^ $(HW) $(LDFLAGS) -o $@
	@echo "Built version: $(VERSION_FLAGS)"

%.hex: %.elf
	$(SZ) $^
	$(CP) -O ihex $^ $(TARGET).hex

clean:
	rm -f *.o src/*.o *.elf  bootloader/*.o $(OBJ)


cbor:
	cd ../../tinycbor/ && make clean
	cd ../../tinycbor/ && make CC="$(CC)" AR=$(AR) \
LDFLAGS="$(LDFLAGS_LIB)" \
CFLAGS="$(CFLAGS) -Os  -DCBOR_PARSER_MAX_RECURSIONS=3"

salty:
	cd ../../crypto/salty/c-api && cargo clean
	cd ../../crypto/salty/c-api && $(MAKE)
