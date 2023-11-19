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

//#include "cbor.h"
//#include "device.h"
//#include "ctaphid.h"
// #include "bsp.h"
//#include "util.h"
//#include "log.h"
//#include "ctap.h"
#include APP_CONFIG
#include "led.h"

// Kyber512
//#include "../../../crypto/kyber/kyber512/api.h"
//#include "../../../crypto/kyber/kyber512/rng.h"

// Falcon512
#include <string.h>
#include <ctype.h>
#include "../../../crypto/falcon/KAT/generator/katrng.h"
#include "../../../crypto/falcon/falcon512cxm4/api.h"

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

void print_time(uint32_t value)
{
    char buffer[12]; // Longest uint32_t string is 10 characters, plus null terminator
    snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)value);
    print_message(buffer);
}

/*
void benchmark_kyber_512() {
    // print_message("Initialising benchmark...\n");

    unsigned char entropy_input[48] = {0};
    for (int i = 0; i < 48; i++)
    {
        entropy_input[i] = i;
    }

    randombytes_init(entropy_input, NULL, 256);

    unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
    unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
    unsigned char ss[CRYPTO_BYTES] = {0};

    uint32_t start_time;
    uint32_t end_time;

    // print_message("... finished!\n");
    // print_message("\nExecuting keypair generation...");

    // Generate a keypair
    start_time = millis();
    crypto_kem_keypair(pk, sk);
    end_time = millis();

    print_message("\nPublic Key:\n");
    print_binary_as_hex(pk, CRYPTO_PUBLICKEYBYTES);
    print_message("\n\nSecret Key:\n");
    print_binary_as_hex(sk, CRYPTO_PUBLICKEYBYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");

    print_message("\nExecuting encryption...");
    // Encrypt a message
    start_time = millis();
    crypto_kem_enc(ct, ss, pk);
    end_time = millis();
    print_message("\nCipher text:\n");
    print_binary_as_hex(ct, CRYPTO_CIPHERTEXTBYTES);
    print_message("\n\nShared secret:\n");
    print_binary_as_hex(ss, CRYPTO_BYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");

    print_message("\nExecuting decryption...");
    // Decrypt the message
    start_time = millis();
    crypto_kem_dec(ss, ct, sk);
    end_time = millis();
    print_message("\n\nDecrypted Shared Secret:\n");
    print_binary_as_hex(ss, CRYPTO_BYTES);

    print_message("\nExecution time: ");
    print_time(end_time - start_time);
    print_message(" ms\n");
}
*/
void benchmark_falcon_512() {
    unsigned long long  mlen, smlen, mlen1;
    int                 ret_val;

    /*
     * Temporary buffers made static to save space on constrained
     * systems (e.g. ARM Cortex M4).
     */
    static unsigned char       seed[48];
    static unsigned char       entropy_input[48];
    static unsigned char       msg[33];
    static unsigned char       pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    unsigned char              m[33], m1[33], sm[33 + CRYPTO_BYTES]; 

    for (int i=0; i<48; i++) {
        entropy_input[i] = i;
    }

    randombytes_init(entropy_input, NULL, 256);
    randombytes(seed, 48);
    print_message("\nseed = ");
    print_binary_as_hex(seed, 48);

    mlen = 33;
    randombytes(msg, mlen);
    print_message("\nmsg = ");
    print_binary_as_hex(msg, mlen);

    randombytes_init(seed, NULL, 256);

    // Generate the public/private keypair
    if ( (ret_val = crypto_sign_keypair(pk, sk)) != 0) {
        print_message("\ncrypto_sign_keypair returned");
        print_time((uint32_t)ret_val);
    }

    print_message("\nPublic Key:\n");
    print_binary_as_hex(pk, CRYPTO_PUBLICKEYBYTES);
    print_message("\n\nSecret Key:\n");
    print_binary_as_hex(sk, CRYPTO_SECRETKEYBYTES);
    
    /**
    * crypto_sign nimmt eine Nachricht (m), deren Länge (mlen), einen privaten Schlüssel (sk)
    * und ein leer initialisiertes Array (sm) entgegen.
    * Sie erzeugt eine Signatur (sm), die die übergebene Nachricht einschließt.
    * Das Ergebnis dieser Funktion ist 0, wenn die Signatur erfolgreich erstellt wurde, andernfalls -1.
    */
    ret_val = crypto_sign(sm, &smlen, msg, mlen, sk);
    if (ret_val != 0) {
        print_message("\ncrypto_sign returned: ");
        print_time((uint32_t)ret_val);
    }

    print_message("\nsmlen = ");
    print_time((uint32_t)smlen);
    print_message("\nsm = ");
    print_binary_as_hex(sm, smlen);
    
    /**
    * crypto_sign_open überprüft eine Signatur (sm), die in Kombination mit einer Nachricht,
    * einem öffentlichen Schlüssel (pk) und einer Signaturlänge (smlen) übergeben wird.
    * Wenn die Signatur gültig ist, wird die Nachricht in den bereitgestellten Pufferspeicher (m) kopiert,
    * und die Funktion gibt 0 zurück. Andernfalls wird -1 zurückgegeben.
    */
    ret_val = crypto_sign_open(m1, &mlen1, sm, smlen, pk);
    if (ret_val != 0) {
        printf("crypto_sign_open returned <%d>\n", ret_val);
    }

    print_message("\nmlen1 = ");
    print_time((uint32_t)mlen1);
    print_message("\nm1 = ");
    print_binary_as_hex(m1, mlen1);
}

void run_benchmark()
{
    // Kyber512
    // benchmark_kyber_512();

    // Falcon512
    benchmark_falcon_512();
}

int main(int argc, char *argv[])
{
    /*
    uint8_t hidmsg[64];
    uint32_t t1 = 0;

    set_logging_mask(
        /*0*/

        /* <- REMOVE 

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
    */
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

    // memset(hidmsg, 0, sizeof(hidmsg));

    while (1)
    {
        /*
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
        */
    }

    // Should never get here
    //usbhid_close();
    //printf1(TAG_GREEN, "done\n");
    return 0;
}

#endif