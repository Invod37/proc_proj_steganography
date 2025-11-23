#include <stdio.h>
#include <stdlib.h>
#include "steg_encode.h"
#include "bmp_utils.h"
#include "steg_utils.h"

int steg_encode(const char *input_bmp, const char *msg_file, const char *output_bmp) {
    printf("--- Running ENCODE ---\n");

    BmpImage img = {0};
    if (bmp_read(input_bmp, &img) != 0) {
        fprintf(stderr, "Error: failed to read BMP '%s' (only 24bpp uncompressed supported)\n", input_bmp);
        return 1;
    }

    size_t msg_len = 0;
    char *message = (char*)read_text_file(msg_file, &msg_len);
    if (!message) {
        fprintf(stderr, "Error: failed to read message file '%s'\n", msg_file);
        bmp_free(&img);
        return 1;
    }

    int rc = steg_encode_message(&img, (unsigned char*)message, msg_len);
    free(message);
    if (rc != 0) {
        fprintf(stderr, "Error: steganography encode failed (maybe message too long for image)\n");
        bmp_free(&img);
        return 1;
    }

    if (bmp_write(output_bmp, &img) != 0) {
        fprintf(stderr, "Error: failed to write output BMP '%s'\n", output_bmp);
        bmp_free(&img);
        return 1;
    }

    bmp_free(&img);
    printf("Encoded message into '%s' -> '%s'\n", input_bmp, output_bmp);
    return 0;
   
}