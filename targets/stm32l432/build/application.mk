include build/common.mk

# ST related
SRC = src/main.c src/init.c src/redirect.c src/flash.c src/rng.c src/led.c src/device.c
SRC += src/fifo.c src/attestation.c src/nfc.c src/ams.c src/sense.c
SRC += src/startup_stm32l432xx.s src/system_stm32l4xx.c
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
SRC += ../../crypto/kyber512/aes256ctr.c \
       ../../crypto/kyber512/cbd.c \
       ../../crypto/kyber512/fips202.c \
       ../../crypto/kyber512/indcpa.c \
       ../../crypto/kyber512/kem.c \
       ../../crypto/kyber512/ntt.c \
       ../../crypto/kyber512/poly.c \
       ../../crypto/kyber512/polyvec.c \
       ../../crypto/kyber512/PQCgenKAT_kem.c \
       ../../crypto/kyber512/reduce.c \
       ../../crypto/kyber512/rng.c \
       ../../crypto/kyber512/sha256.c \
       ../../crypto/kyber512/sha512.c \
       ../../crypto/kyber512/symmetric-aes.c \
       ../../crypto/kyber512/symmetric-shake.c \
       ../../crypto/kyber512/verify.c

# mbed TLS library
SRC += ../../mbedtls/library/aes.c ../../mbedtls/library/aesni.c ../../mbedtls/library/arc4.c \
       ../../mbedtls/library/asn1parse.c ../../mbedtls/library/asn1write.c ../../mbedtls/library/base64.c \
       ../../mbedtls/library/bignum.c ../../mbedtls/library/blowfish.c ../../mbedtls/library/camellia.c \
       ../../mbedtls/library/ccm.c ../../mbedtls/library/cipher.c ../../mbedtls/library/cipher_wrap.c \
       ../../mbedtls/library/cmac.c ../../mbedtls/library/ctr_drbg.c ../../mbedtls/library/des.c \
       ../../mbedtls/library/dhm.c ../../mbedtls/library/ecdh.c ../../mbedtls/library/ecdsa.c \
       ../../mbedtls/library/ecjpake.c ../../mbedtls/library/ecp.c ../../mbedtls/library/ecp_curves.c \
       ../../mbedtls/library/entropy.c ../../mbedtls/library/entropy_poll.c ../../mbedtls/library/error.c \
       ../../mbedtls/library/gcm.c ../../mbedtls/library/havege.c ../../mbedtls/library/hmac_drbg.c \
       ../../mbedtls/library/md.c ../../mbedtls/library/md2.c ../../mbedtls/library/md4.c \
       ../../mbedtls/library/md5.c ../../mbedtls/library/md_wrap.c ../../mbedtls/library/memory_buffer_alloc.c \
       ../../mbedtls/library/net_sockets.c ../../mbedtls/library/oid.c ../../mbedtls/library/padlock.c \
       ../../mbedtls/library/pem.c ../../mbedtls/library/pk.c ../../mbedtls/library/pk_wrap.c \
       ../../mbedtls/library/pkcs12.c ../../mbedtls/library/pkcs5.c ../../mbedtls/library/pkparse.c \
       ../../mbedtls/library/pkwrite.c ../../mbedtls/library/platform.c ../../mbedtls/library/ripemd160.c \
       ../../mbedtls/library/rsa.c ../../mbedtls/library/rsa_internal.c ../../mbedtls/library/sha1.c \
       ../../mbedtls/library/sha256.c ../../mbedtls/library/sha512.c ../../mbedtls/library/ssl_cache.c \
       ../../mbedtls/library/ssl_ciphersuites.c ../../mbedtls/library/ssl_cli.c ../../mbedtls/library/ssl_cookie.c \
       ../../mbedtls/library/ssl_srv.c ../../mbedtls/library/ssl_ticket.c ../../mbedtls/library/ssl_tls.c \
       ../../mbedtls/library/threading.c ../../mbedtls/library/timing.c ../../mbedtls/library/version.c \
       ../../mbedtls/library/version_features.c ../../mbedtls/library/xtea.c



OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.s=.o)

INC = -Isrc/ -Isrc/cmsis/ -Ilib/ -Ilib/usbd/

INC += -I../../fido2/ -I../../fido2/extensions
INC += -I../../tinycbor/src -I../../crypto/sha256 -I../../crypto/micro-ecc
INC += -I../../crypto/tiny-AES-c
INC += -I../../crypto/cifra/src -I../../crypto/cifra/src/ext
INC += -I../../crypto/salty/c-api

# mbed TLS include files
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

CFLAGS=$(INC) -I../../crypto/kyber512/ -c $(DEFINES)   -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fdata-sections -ffunction-sections \
	-fomit-frame-pointer $(HW) -g $(VERSION_FLAGS)
LDFLAGS_LIB=$(HW) $(SEARCH) -specs=nano.specs  -specs=nosys.specs  -Wl,--gc-sections -lnosys
LDFLAGS=$(HW) $(LDFLAGS_LIB) -T$(LDSCRIPT) -Wl,-Map=$(TARGET).map,--cref -Wl,-Bstatic -ltinycbor -lsalty

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
