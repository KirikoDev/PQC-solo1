CC=/usr/bin/gcc
CFLAGS += -O3 -march=native -fomit-frame-pointer -I/usr/local/Cellar/openssl@3/3.1.1_1/include
LDFLAGS=-L/usr/local/Cellar/openssl@3/3.1.1_1/lib -lcrypto

SOURCES= cbd.c fips202.c indcpa.c kem.c ntt.c poly.c polyvec.c PQCgenKAT_kem.c reduce.c rng.c verify.c symmetric-shake.c
HEADERS= api.h cbd.h fips202.h indcpa.h ntt.h params.h poly.h polyvec.h reduce.h rng.h verify.h symmetric.h

PQCgenKAT_kem: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

.PHONY: clean

clean:
	-rm PQCgenKAT_kem
