#include "../include/steg_decode.h"
#include <stdlib.h>
#include <string.h>

static size_t abs_i(int v) { 
    return v < 0 ? (size_t)(-v) : (size_t)v; 
}

static inline unsigned char get_lsb(unsigned char b) {
    return b & 1u;
}

int steg_decode_message(const BmpImage *img, unsigned char **out_message, size_t *out_len) {
    if (!img || !img->data) return -1;
    if (!out_message || !out_len) return -1;

    size_t width = (size_t)img->width;
    size_t height = abs_i(img->height);
    unsigned char *data = img->data;
    int stride = img->stride;

    
    unsigned char temp[4] = {0};
    size_t idx = 0;
    unsigned bit = 0;

    for (size_t row = 0; row < height && idx < 4; ++row) {
        for (size_t i = 0; i < 3u * width && idx < 4; ++i) {
            temp[idx] |= (get_lsb(data[row * stride + i]) << bit);
            bit++;
            if (bit == 8) { bit = 0; idx++; }
        }
    }

    size_t msg_len = temp[0] | (temp[1] << 8) | (temp[2] << 16) | (temp[3] << 24);
    if (msg_len == 0) return -2;

 
    unsigned char *msg = (unsigned char*)calloc(msg_len + 1, 1);
    if (!msg) return -3;

    
    idx = 0;
    bit = 0;
    size_t skip = 32; 

    for (size_t row = 0; row < height && idx < msg_len; ++row) {
        for (size_t i = 0; i < 3u * width && idx < msg_len; ++i) {
            if (skip > 0) { skip--; continue; }
            
            msg[idx] |= (get_lsb(data[row * stride + i]) << bit);
            bit++;
            if (bit == 8) { bit = 0; idx++; }
        }
    }

    *out_message = msg;
    *out_len = msg_len;
    return 0;
}