// Copyright 2019 SoloKeys Developers
//
// Licensed under the Apache License, Version 2.0, <LICENSE-APACHE or
// http://apache.org/licenses/LICENSE-2.0> or the MIT license <LICENSE-MIT or
// http://opensource.org/licenses/MIT>, at your option. This file may not be
// copied, modified, or distributed except according to those terms.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "cbor.h"
#include "device.h"
#include "ctaphid.h"
// #include "bsp.h"
#include "util.h"
#include "log.h"
#include "ctap.h"
#include APP_CONFIG
#include "led.h"

// Kyber512
#include "../../../crypto/kyber/kyber512/api.h"
#include "../../../crypto/kyber/kyber512/rng.h"

// Kyber512-90s
// #include "../../../crypto/kyber/kyber512-90s/api.h"
// #include "../../../crypto/kyber/kyber512-90s/rng.h"

// Kyber768
// #include "../../../crypto/kyber/kyber768/api.h"
// #include "../../../crypto/kyber/kyber768/rng.h"

// Falcon512
// #include <string.h>
// #include <ctype.h>
// #include "../../../crypto/falcon/KAT/generator/katrng.h"
// #include "../../../crypto/falcon/falcon512cxm4/api.h"

// Dilithium2
// #include "../../../crypto/dilithium/dilithium2/api.h"
// #include "../../../crypto/dilithium/dilithium2/rng.h"

// Dilithium2-AES
// #include "../../../crypto/dilithium/dilithium2-AES/api.h"
// #include "../../../crypto/dilithium/dilithium2-AES/rng.h"

// Sphinx sha256 128f simple
// #include "../../../crypto/sphincs-sha256-128f-simple/api.h"
// #include "../../../crypto/sphincs-sha256-128f-simple/rng.h"

#if !defined(TEST)

void print_message(const char *message)
{
    // Calculate the length of the message
    size_t len = strlen(message);

    // Transmit the message over USB CDC
    int result = CDC_Transmit_FS((uint8_t *)message, len);
    while (result == 1)
    {
        result = CDC_Transmit_FS((uint8_t *)message, len);
    }
}

void print_time(uint32_t value)
{
    char buffer[12]; // Longest uint32_t string is 10 characters, plus null terminator
    snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)value);
    print_message(buffer);
}

void print_binary_as_hex(const unsigned char *data, size_t length)
{
    char hex_str[3]; // Two characters for each byte plus the null terminator
    hex_str[2] = '\0';

    for (size_t i = 0; i < length; i++)
    {
        snprintf(hex_str, sizeof(hex_str), "%02X", data[i]);
        print_message(hex_str);
    }
    print_message("\n");
}

#define MAX_ITERATIONS 1000
void benchmark_kyber_512() {
    // print_time(MAX_ITERATIONS);
    // print_message(" Iterations in progress...\n");
    // print_message("Initialising benchmark...\n");

    unsigned char entropy_input[48] = {0};
    for (int i = 0; i < 48; i++)
    {
        entropy_input[i] = i;
    }

    randombytes_init(entropy_input, NULL, 256);

    uint32_t start_time, end_time, total_start_time, total_end_time;
    uint32_t time_keypair = 0, time_enc = 0, time_dec = 0;
    int correct = 0, failed = 0;

    // print_message("... finished!\n");
    // print_message("\nExecuting keypair generation...");

    total_start_time = millis();
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
        unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
        unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
        unsigned char ss[CRYPTO_BYTES] = {0}, ss_dec[CRYPTO_BYTES] = {0};

        print_message("\nIteration ");
        print_time(i+1);
        // Generate a keypair
        start_time = millis();
        crypto_kem_keypair(pk, sk);
        end_time = millis();
        time_keypair += (end_time - start_time);

        // print_message("\nPublic Key:\n");
        // print_binary_as_hex(pk, CRYPTO_PUBLICKEYBYTES);
        // print_message("\n\nSecret Key:\n");
        // print_binary_as_hex(sk, CRYPTO_PUBLICKEYBYTES);

        print_message("\nKeypair time: ");
        print_time(end_time - start_time);
        // print_message(" ms\n");

        // print_message("\nExecuting encryption...");
        // Encrypt a message
        start_time = millis();
        crypto_kem_enc(ct, ss, pk);
        end_time = millis();
        time_enc += (end_time - start_time);
        // print_message("\nCipher text:\n");
        // print_binary_as_hex(ct, CRYPTO_CIPHERTEXTBYTES);
        // print_message("\n\nShared secret:\n");
        // print_binary_as_hex(ss, CRYPTO_BYTES);

        print_message("\nEnc time: ");
        print_time(end_time - start_time);
        // print_message(" ms\n");

        // print_message("\nExecuting decryption...");
        // Decrypt the message
        start_time = millis();
        crypto_kem_dec(ss_dec, ct, sk);
        end_time = millis();
        time_dec += (end_time - start_time);
        // print_message("\n\nDecrypted Shared Secret:\n");
        // print_binary_as_hex(ss, CRYPTO_BYTES);

        print_message("\nDec time: ");
        print_time(end_time - start_time);
        // print_message(" ms\n");

        if (memcmp(ss, ss_dec, CRYPTO_BYTES) == 0) {
            correct++;
        } else {
            failed++;
        }
    }
    total_end_time = millis();

    // print_message("\nCorrect: ");
    // print_time(correct);
    // print_message(", Failed: ");
    // print_time(failed);
    // print_message("\nAverage time keypair generation: ");
    // print_time(time_keypair / MAX_ITERATIONS);
    // print_message(" ms\nAverage time encryption: ");
    // print_time(time_enc / MAX_ITERATIONS);
    // print_message(" ms\nAverage time decryption: ");
    // print_time(time_dec / MAX_ITERATIONS);
    // print_message("\nTotal time: ");
    // print_time(total_end_time - total_start_time);
    // print_message(" ms\n");
}

// void benchmark_falcon_512() {
//     unsigned long long  mlen, smlen, mlen1;
//     int                 ret_val;

//     /*
//      * Temporary buffers made static to save space on constrained
//      * systems (e.g. ARM Cortex M4).
//      */
//     static unsigned char       seed[48];
//     static unsigned char       entropy_input[48];
//     static unsigned char       msg[33];
//     static unsigned char       pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
//     unsigned char              m[33], m1[33], sm[33 + CRYPTO_BYTES]; 

//     for (int i=0; i<48; i++) {
//         entropy_input[i] = i;
//     }

//     randombytes_init(entropy_input, NULL, 256);
//     randombytes(seed, 48);
//     print_message("\nseed = ");
//     print_binary_as_hex(seed, 48);

//     mlen = 33;
//     randombytes(msg, mlen);
//     print_message("\nmsg = ");
//     print_binary_as_hex(msg, mlen);

//     randombytes_init(seed, NULL, 256);

//     // Generate the public/private keypair
//     if ( (ret_val = crypto_sign_keypair(pk, sk)) != 0) {
//         print_message("\ncrypto_sign_keypair returned");
//         print_time((uint32_t)ret_val);
//     }

//     print_message("\nPublic Key:\n");
//     print_binary_as_hex(pk, CRYPTO_PUBLICKEYBYTES);
//     print_message("\n\nSecret Key:\n");
//     print_binary_as_hex(sk, CRYPTO_SECRETKEYBYTES);
    
//     /**
//     * crypto_sign nimmt eine Nachricht (m), deren Länge (mlen), einen privaten Schlüssel (sk)
//     * und ein leer initialisiertes Array (sm) entgegen.
//     * Sie erzeugt eine Signatur (sm), die die übergebene Nachricht einschließt.
//     * Das Ergebnis dieser Funktion ist 0, wenn die Signatur erfolgreich erstellt wurde, andernfalls -1.
//     */
//     ret_val = crypto_sign(sm, &smlen, msg, mlen, sk);
//     if (ret_val != 0) {
//         print_message("\ncrypto_sign returned: ");
//         print_time((uint32_t)ret_val);
//     }

//     print_message("\nsmlen = ");
//     print_time((uint32_t)smlen);
//     print_message("\nsm = ");
//     print_binary_as_hex(sm, smlen);
    
//     /**
//     * crypto_sign_open überprüft eine Signatur (sm), die in Kombination mit einer Nachricht,
//     * einem öffentlichen Schlüssel (pk) und einer Signaturlänge (smlen) übergeben wird.
//     * Wenn die Signatur gültig ist, wird die Nachricht in den bereitgestellten Pufferspeicher (m) kopiert,
//     * und die Funktion gibt 0 zurück. Andernfalls wird -1 zurückgegeben.
//     */
//     ret_val = crypto_sign_open(m1, &mlen1, sm, smlen, pk);
//     if (ret_val != 0) {
//         printf("crypto_sign_open returned <%d>\n", ret_val);
//     }

//     print_message("\nmlen1 = ");
//     print_time((uint32_t)mlen1);
//     print_message("\nm1 = ");
//     print_binary_as_hex(m1, mlen1);
// }
/*
#define MLEN 256
void benchmark_dilithium2() {
    int ret;
    size_t mlen, smlen;
    uint8_t m[MLEN] = {0};
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t m2[MLEN + CRYPTO_BYTES];
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint32_t start_time, end_time;

    // randombytes(m, MLEN);

    // Sign a keypair
    print_message("\nExecuting signing...");
    start_time = millis();
    crypto_sign_keypair(pk, sk);
    end_time = millis();

    print_message("\nPublic Key:\n");
    print_binary_as_hex(pk, CRYPTO_PUBLICKEYBYTES);
    print_message("\n\nSecret Key:\n");
    print_binary_as_hex(sk, CRYPTO_SECRETKEYBYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");

    // Sign a message
    start_time = millis();
    crypto_sign(sm, &smlen, m, MLEN, sk);
    end_time = millis();
    print_message("\nSigned message:\n");
    print_binary_as_hex(sm, smlen + CRYPTO_BYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");

    print_message("\nExecuting verification...");

    // Verify the signed message
    start_time = millis();
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
    end_time = millis();

    if(ret) {
        print_message("\nVerification failed!");
        return -1;
    }   

    if(mlen != MLEN) {
        print_message("\nMessage lengths dont match");
        return -1;
    }

    print_message("\nVerified message:\n");
    print_binary_as_hex(m2, mlen + CRYPTO_BYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");
}
    
*/
/*
#define SPX_MLEN 32
#define NTESTS 10
void benchmark_sphincs_sha_256_128f_simple() {
    setbuf(stdout, NULL);

    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    unsigned char *m = malloc(SPX_MLEN);
    unsigned char *sm = malloc(SPX_BYTES + SPX_MLEN);
    unsigned char *mout = malloc(SPX_BYTES + SPX_MLEN);

    unsigned char fors_pk[SPX_FORS_PK_BYTES];
    unsigned char fors_m[SPX_FORS_MSG_BYTES];
    unsigned char fors_sig[SPX_FORS_BYTES];
    unsigned char addr[SPX_ADDR_BYTES];

    unsigned char wots_sig[SPX_WOTS_BYTES];
    unsigned char wots_m[SPX_N];
    unsigned char wots_pk[SPX_WOTS_PK_BYTES];

    unsigned long long smlen;
    unsigned long long mlen;
    unsigned long long t[NTESTS+1];
    struct timespec start, stop;
    double result;
    int i;

    randombytes(m, SPX_MLEN);
    randombytes(addr, SPX_ADDR_BYTES);

    // Generate keypair
    crypto_sign_keypair(pk, sk);
    print_message("Keypair generated.\n");
    print_message("Public key: ");
    print_binary_as_hex(pk, SPX_PK_BYTES);
    print_message("Secret key: ");
    print_binary_as_hex(sk, SPX_SK_BYTES);

    // WOTS pk gen
    wots_gen_pk(wots_pk, sk, pk, (uint32_t *) addr);

    // Signing
    crypto_sign(sm, &smlen, m, SPX_MLEN, sk);

    // FORS signing
    fors_sign(fors_sig, fors_pk, fors_m, sk, pk, (uint32_t *) addr);

    // WOTS signing
    wots_sign(wots_sig, wots_m, sk, pk, (uint32_t *) addr);

    // WOTS pk gen
    wots_gen_pk(wots_pk, sk, pk, (uint32_t *) addr);

    // Verify 
    crypto_sign_open(mout, &mlen, sm, smlen, pk);

    // TODO: Add print statements

    free(m);
    free(sm);
    free(mout);
}
*/
void run_benchmark()
{
    // Initialize message with random data
    // unsigned char entropy_input[48] = {0};
    // for (int i = 0; i < 48; i++)
    // {
    //     entropy_input[i] = i;
    // }
    // randombytes_init(entropy_input, NULL, 256);

    // Kyber512
    benchmark_kyber_512();
    
    // Falcon512
    // benchmark_falcon_512();

    // Dilithium2
    // benchmark_dilithium2();

    // Sphincs sha256 128f simple
    // benchmark_sphincs_sha_256_128f_simple();
}

int main(int argc, char *argv[])
{
    
    uint8_t hidmsg[64];
    uint32_t t1 = 0;

    set_logging_mask(
        /*0*/

        // TAG_GEN|
        //  TAG_MC |
        //  TAG_GA |
        TAG_WALLET |
        TAG_STOR |
        // TAG_NFC_APDU |
        TAG_NFC |
        // TAG_CP |
        //  TAG_CTAP|
        // TAG_HID|
        TAG_U2F |
        // TAG_PARSE |
        // TAG_TIME|
        //  TAG_DUMP|
        TAG_GREEN |
        TAG_RED |
        TAG_EXT |
        TAG_CCID |
        TAG_ERR);

    device_init(argc, argv);

    // // Initialize USB Device Library
    // USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FAST);

    // // Add Supported Class
    // USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

    // // Add CDC Interface Class
    // USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    // // Start Device Process
    // USBD_Start(&hUsbDeviceFS);

    // led_rgb(0xFF0000);
    // delay(1000);
    led_rgb(0x00FF00);
    delay(2000);

    // Running benchmark after initialization
    run_benchmark();

    memset(hidmsg, 0, sizeof(hidmsg));

    while (1)
    {
        if (millis() - t1 > HEARTBEAT_PERIOD)
        {
            heartbeat();
            t1 = millis();
        }

        device_manage();

        if (usbhid_recv(hidmsg) > 0)
        {
            ctaphid_handle_packet(hidmsg);
            memset(hidmsg, 0, sizeof(hidmsg));
        }
        else
        {
        }
        ctaphid_check_timeouts();
    }

    // Should never get here
    usbhid_close();
    printf1(TAG_GREEN, "done\n");
    return 0;
}

#endif