#include "../../../mbedtls/include/mbedtls/platform_time.h"
#include <time.h>

mbedtls_time_t mbedtls_time(time_t *timer)
{
    *timer = time(NULL);
    return *timer;
}

mbedtls_time_t mbedtls_timegm(struct tm *tm)
{
    time_t t = mktime(tm);
    if (t == -1) {
        return (mbedtls_time_t)-1;
    }
    return (mbedtls_time_t)t;
}

mbedtls_time_t mbedtls_get_time()
{
    time_t t;
    mbedtls_time(&t);
    return (mbedtls_time_t)t;
}