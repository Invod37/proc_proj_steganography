#include "steg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

static inline unsigned char get_lsb(unsigned char b) {
    return b & 1u;
}

static size_t abs_i(int v) { return v < 0 ? (size_t)(-v) : (size_t)v; }

static inline unsigned char set_lsb(unsigned char b, unsigned bit) {
    b &= 0xFEu;
    b |= (bit & 1u);
    return b;
}

int steg_encode_message(BmpImage *img, const unsigned char *message, size_t message_len) {
    if (!img || !img->data) return -1;
    if (!message && message_len != 0) return -1;

    size_t width = (size_t)img->width;
    size_t height = abs_i(img->height);

    size_t bits_capacity = width * height * 3u;

    size_t total_bytes = 4u + message_len;
    size_t needed_bits = total_bytes * 8u;

    if (needed_bits > bits_capacity) {
        return -2;
    }

    unsigned char *payload = (unsigned char*)malloc(total_bytes);
    if (!payload) return -3;

    payload[0] = (unsigned char)((message_len) & 0xFFu);
    payload[1] = (unsigned char)((message_len >> 8) & 0xFFu);
    payload[2] = (unsigned char)((message_len >> 16) & 0xFFu);
    payload[3] = (unsigned char)((message_len >> 24) & 0xFFu);

    if (message_len)
        memcpy(payload + 4, message, message_len);

    size_t pay_idx = 0;
    unsigned bit_idx = 0;
    unsigned char *data = img->data;
    int stride = img->stride;

    for (size_t row = 0; row < height && pay_idx < total_bytes; ++row) {
        size_t base = row * (size_t)stride;
        size_t row_bytes = 3u * width;
        for (size_t i = 0; i < row_bytes && pay_idx < total_bytes; ++i) {
            unsigned bit = (payload[pay_idx] >> bit_idx) & 1u;
            data[base + i] = set_lsb(data[base + i], bit);
            bit_idx++;
            if (bit_idx == 8) {
                bit_idx = 0;
                pay_idx++;
            }
        }
    }

    free(payload);
    return 0;
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

char *read_text_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz < 0) { fclose(f); return NULL; }
    fseek(f, 0, SEEK_SET);
    char *buf = (char*)malloc((size_t)sz + 1);
    if (!buf) { fclose(f); return NULL; }
    size_t n = fread(buf, 1, (size_t)sz, f);
    fclose(f);
    buf[n] = '\0';
    if (out_len) *out_len = n;
    return buf;
}