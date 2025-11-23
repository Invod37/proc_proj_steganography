#include <stdio.h>
#include <stdlib.h>
#include "steg_decode.h"
#include "bmp_utils.h"
#include "steg_utils.h"

int steg_decode(const char *input_bmp, const char *msg_file) {
    printf("--- Running DECODE ---\n");

    BmpImage img = {0};
    if (bmp_read(input_bmp, &img) != 0) {
        fprintf(stderr, "Error: failed to read BMP '%s'\n", input_bmp);
        return 1;
    }

    unsigned char *message = NULL;
    size_t msg_len = 0;
    
    int rc = steg_decode_message(&img, &message, &msg_len);
    bmp_free(&img);
    
    if (rc != 0) {
        fprintf(stderr, "Error: failed to decode message from '%s'\n", input_bmp);
        return 1;
    }

    FILE *f = fopen(msg_file, "wb");
    if (!f) {
        fprintf(stderr, "Error: failed to open output file '%s'\n", msg_file);
        free(message);
        return 1;
    }

    fwrite(message, 1, msg_len, f);
    fclose(f);
    free(message);

    printf("Decoded message from '%s' -> '%s' (%zu bytes)\n", input_bmp, msg_file, msg_len);
 
    return 0;
}