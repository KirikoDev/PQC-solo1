//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//

#include <string.h>
#include "rng.h"
#include "mbedtls/aes.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

#define RNG_FAILURE -1

AES256_CTR_DRBG_struct DRBG_ctx;
mbedtls_entropy_context entropy;

void AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer);

int seedexpander_init(AES_XOF_struct *ctx,
                      unsigned char *seed,
                      unsigned char *diversifier,
                      unsigned long maxlen)
{
    if (maxlen >= 0x100000000)
        return RNG_BAD_MAXLEN;

    ctx->length_remaining = maxlen;

    memcpy(ctx->key, seed, 32);

    memcpy(ctx->ctr, diversifier, 8);
    ctx->ctr[11] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[10] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[9] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[8] = maxlen % 256;
    memset(ctx->ctr + 12, 0x00, 4);

    ctx->buffer_pos = 16;
    memset(ctx->buffer, 0x00, 16);

    return RNG_SUCCESS;
}

int seedexpander(AES_XOF_struct *ctx, unsigned char *x, unsigned long xlen)
{
    unsigned long offset;

    if (x == NULL)
        return RNG_BAD_OUTBUF;
    if (xlen >= ctx->length_remaining)
        return RNG_BAD_REQ_LEN;

    ctx->length_remaining -= xlen;

    offset = 0;
    while (xlen > 0)
    {
        if (xlen <= (16 - ctx->buffer_pos))
        { // buffer has what we need
            memcpy(x + offset, ctx->buffer + ctx->buffer_pos, xlen);
            ctx->buffer_pos += xlen;

            return RNG_SUCCESS;
        }

        // take what's in the buffer
        memcpy(x + offset, ctx->buffer + ctx->buffer_pos, 16 - ctx->buffer_pos);
        xlen -= 16 - ctx->buffer_pos;
        offset += 16 - ctx->buffer_pos;

        AES256_ECB(ctx->key, ctx->ctr, ctx->buffer);
        ctx->buffer_pos = 0;

        // increment the counter
        for (int i = 15; i >= 12; i--)
        {
            if (ctx->ctr[i] == 0xff)
                ctx->ctr[i] = 0x00;
            else
            {
                ctx->ctr[i]++;
                break;
            }
        }
    }

    return RNG_SUCCESS;
}

void AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer)
{
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);

    int ret = mbedtls_aes_setkey_enc(&ctx, key, 256);
    if (ret != 0)
    {
        printf("Failed to set AES key. Error code: %d\n", ret);
        return;
    }

    ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, ctr, buffer);
    if (ret != 0)
    {
        printf("Failed to encrypt with AES. Error code: %d\n", ret);
        return;
    }

    mbedtls_aes_free(&ctx);
}

void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength)
{
    unsigned char seed_material[48];

    memcpy(seed_material, entropy_input, 48);
    if (personalization_string)
        for (int i = 0; i < 48; i++)
            seed_material[i] ^= personalization_string[i];
    memset(DRBG_ctx.Key, 0x00, 32);
    memset(DRBG_ctx.V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter = 1;

    mbedtls_entropy_init(&entropy);
}

int randombytes(unsigned char *x, unsigned long long xlen)
{
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);

    // Use a constant personalization string. In a real application, you might want to use a more random string
    const char *pers = "ctr_drbg";
    int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
    if (ret != 0)
    {
        printf("Failed to seed CTR_DRBG. Error code: %d\n", ret);
        return RNG_FAILURE;
    }

    ret = mbedtls_ctr_drbg_random(&ctr_drbg, x, xlen);
    if (ret != 0)
    {
        printf("Failed to generate random bytes. Error code: %d\n", ret);
        return RNG_FAILURE;
    }

    mbedtls_ctr_drbg_free(&ctr_drbg);

    return RNG_SUCCESS;
}

void AES256_CTR_DRBG_Update(unsigned char *provided_data,
                            unsigned char *Key,
                            unsigned char *V)
{
    unsigned char temp[48];

    for (int i = 0; i < 3; i++)
    {
        // increment V
        for (int j = 15; j >= 0; j--)
        {
            if (V[j] == 0xff)
                V[j] = 0x00;
            else
            {
                V[j]++;
                break;
            }
        }

        AES256_ECB(Key, V, temp + 16 * i);
    }
    if (provided_data != NULL)
        for (int i = 0; i < 48; i++)
            temp[i] ^= provided_data[i];
    memcpy(Key, temp, 32);
    memcpy(V, temp + 32, 16);
}
