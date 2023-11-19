#include "version.h"
#include "app.h"

const version_t firmware_version 
#ifdef SOLO
__attribute__ ((section (".flag"))) __attribute__ ((__used__)) 
#endif
    =  {
      .major = 0,
      .minor = 0,
      .patch = 0,
      .reserved = 0
    };


// from tinycbor, for a quick static_assert
#include <compilersupport_p.h>
cbor_static_assert(sizeof(version_t) == 4);
