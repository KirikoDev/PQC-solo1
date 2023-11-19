#include "../../../mbedtls/library/entropy_poll.h"
#include "stm32l4xx_ll_rng.h"
#include "stm32l432xx.h"
#include <stdint.h>
#include <stddef.h>

int mbedtls_platform_entropy_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    // // Ensure the requested length is not greater than what we can provide
    // if (len > SOLOKEY_RNG_MAX_OUTPUT_LEN)
    // {
    //     return -1; // Error: requested length is too large
    // }

    // Generate random bytes using rng_get_bytes
    rng_get_bytes(output, len);

    // Set the actual length of generated random data
    *olen = len;

    return 0; // Success
}

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    // // Ensure the requested length is not greater than what we can provide
    // if (len > SOLOKEY_RNG_MAX_OUTPUT_LEN)
    // {
    //     return -1; // Error: requested length is too large
    // }

    // Generate random bytes using rng_get_bytes
    rng_get_bytes(output, len);

    // Set the actual length of generated random data
    *olen = len;

    return 0; // Success
}
