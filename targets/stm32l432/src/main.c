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
#include "../../../crypto/kyber512/api.h"

#if !defined(TEST)

void run_benchmark()
{
    unsigned char pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES], ss1[CRYPTO_BYTES], ss2[CRYPTO_BYTES];

    uint32_t start_time, end_time, elapsed_time;

    start_time = HAL_GetTick(); // Getting system tick for STM32L432

    // Generate a keypair
    crypto_kem_keypair(pk, sk);

    // Encrypt a message
    crypto_kem_enc(ct, ss1, pk);

    // Decrypt the message
    crypto_kem_dec(ss2, ct, sk);

    end_time = HAL_GetTick(); // Getting system tick for STM32L432

    elapsed_time = end_time - start_time;

    printf("Kyber benchmark elapsed time: %lu\n", elapsed_time); // Printing the elapsed time to console
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