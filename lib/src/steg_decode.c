#include "../include/steg_decode.h"
#include <stdlib.h>
#include <string.h>

static size_t abs_i(int v) { 
    return v < 0 ? (size_t)(-v) : (size_t)v; 
}

static inline unsigned char get_lsb(unsigned char b) {
    return b & 1u;
}

