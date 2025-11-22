#ifndef STEG_DECODE_H
#define STEG_DECODE_H

#include <stddef.h>
#include <stdint.h>
#include "bmp_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int steg_decode_message(const BmpImage *img, unsigned char **out_message, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif 