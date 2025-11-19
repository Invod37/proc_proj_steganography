#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/include/steg_encode.h"
#include "../lib/include/bmp_utils.h"

static char *read_text_file(const char *path, size_t *out_len) {
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

int main(int argc, char **argv) {
    const char *in_bmp = argc > 1 ? argv[1] : "data/input.bmp";
    const char *msg_txt = argc > 2 ? argv[2] : "data/message.txt";
    const char *out_bmp = argc > 3 ? argv[3] : "data/encoded.bmp";

    BmpImage img = {0};
    if (bmp_read(in_bmp, &img) != 0) {
        fprintf(stderr, "Error: failed to read BMP '%s' (only 24bpp uncompressed supported)\n", in_bmp);
        return 1;
    }

    size_t msg_len = 0;
    char *message = read_text_file(msg_txt, &msg_len);
    if (!message) {
        fprintf(stderr, "Error: failed to read message file '%s'\n", msg_txt);
        bmp_free(&img);
        return 1;
    }

    int rc = steg_encode_message(&img, (const unsigned char*)message, msg_len);
    free(message);
    if (rc != 0) {
        fprintf(stderr, "Error: steganography encode failed (maybe message too long for image)\n");
        bmp_free(&img);
        return 1;
    }

    if (bmp_write(out_bmp, &img) != 0) {
        fprintf(stderr, "Error: failed to write output BMP '%s'\n", out_bmp);
        bmp_free(&img);
        return 1;
    }

    bmp_free(&img);
    printf("Encoded message into '%s' -> '%s'\n", in_bmp, out_bmp);
    return 0;
}
