#ifndef STEG_ENCODE_H
#define STEG_ENCODE_H

#include <stddef.h>
#include <stdint.h>
#include "bmp_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int steg_encode_message(BmpImage *img, const unsigned char *message, size_t message_len);

#ifdef __cplusplus
}
#endif

#endif
