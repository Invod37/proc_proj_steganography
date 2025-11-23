#ifndef STEG_UTILS_H
#define STEG_UTILS_H

#include <stddef.h>
#include <stdint.h>
#include "bmp_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int steg_encode_message(BmpImage *img, const unsigned char *message, size_t message_len);

int steg_decode_message(const BmpImage *img, unsigned char **out_message, size_t *out_len);

char *read_text_file(const char *path, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif
