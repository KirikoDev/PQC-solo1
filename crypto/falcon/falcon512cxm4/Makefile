.POSIX:

CC = /usr/local/bin/arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall
LD = /usr/local/bin/arm-none-eabi-gcc
LDFLAGS =
LIBS = -specs=nano.specs -specs=nosys.specs

OBJ1 = build/codec.o build/common.o build/fft.o build/fpr.o build/keygen.o build/nist.o build/rng.o build/shake.o build/sign.o build/vrfy.o

OBJ2 = build/PQCgenKAT_sign.o build/katrng.o

HEAD1 = api.h fpr.h inner.h
HEAD2 = api.h ../KAT/generator/katrng.h

all: build build/kat512cxm4

build:
	-mkdir build

clean:
	-rm -f build/kat512cxm4 $(OBJ1) $(OBJ2)

build/kat512cxm4: $(OBJ1) $(OBJ2)
	$(LD) $(LDFLAGS) -o build/kat512cxm4 $(OBJ1) $(OBJ2) $(LIBS)

build/codec.o: codec.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/codec.o codec.c

build/common.o: common.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/common.o common.c

build/fft.o: fft.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/fft.o fft.c

build/fpr.o: fpr.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/fpr.o fpr.c

build/keygen.o: keygen.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/keygen.o keygen.c

build/nist.o: nist.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/nist.o nist.c

build/rng.o: rng.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/rng.o rng.c

build/shake.o: shake.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/shake.o shake.c

build/sign.o: sign.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/sign.o sign.c

build/vrfy.o: vrfy.c $(HEAD1)
	$(CC) $(CFLAGS) -c -o build/vrfy.o vrfy.c

build/PQCgenKAT_sign.o: ../KAT/generator/PQCgenKAT_sign.c $(HEAD2)
	$(CC) $(CFLAGS) -I . -DALGNAME=falcon512cxm4 -c -o build/PQCgenKAT_sign.o ../KAT/generator/PQCgenKAT_sign.c

build/katrng.o: ../KAT/generator/katrng.c $(HEAD2)
	$(CC) $(CFLAGS) -I . -c -o build/katrng.o ../KAT/generator/katrng.c
