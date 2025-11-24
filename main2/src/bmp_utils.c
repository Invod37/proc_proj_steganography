#include "bmp_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compute_stride(int width) {
    int row_bytes = width * 3;
    int padding = (4 - (row_bytes % 4)) % 4;
    return row_bytes + padding;
}

int bmp_read(const char *path, BmpImage *out) {
    if (!out) return -1;
    memset(out, 0, sizeof(*out));

    FILE *f = fopen(path, "rb");
    if (!f) return -1;

    BmpFileHeader fh;
    BmpInfoHeader ih;

    if (fread(&fh, sizeof(fh), 1, f) != 1) { fclose(f); return -1; }
    if (fread(&ih, sizeof(ih), 1, f) != 1) { fclose(f); return -1; }

    if (fh.bfType != 0x4D42) {
        fclose(f);
        return -1;
    }
    if (ih.biSize != 40 || ih.biPlanes != 1 || ih.biBitCount != 24 || ih.biCompression != 0) {
        fclose(f);
        return -1;
    }

    int width = ih.biWidth;
    int height = ih.biHeight;
    int abs_height = height < 0 ? -height : height;

    int stride = compute_stride(width);
    size_t data_size = (size_t)abs_height * (size_t)stride;

    unsigned char *data = (unsigned char*)malloc(data_size);
    if (!data) { fclose(f); return -1; }

    if (fseek(f, (long)fh.bfOffBits, SEEK_SET) != 0) {
        free(data);
        fclose(f);
        return -1;
    }
    size_t n = fread(data, 1, data_size, f);
    fclose(f);
    if (n != data_size) {
        free(data);
        return -1;
    }

    out->width = width;
    out->height = height;
    out->stride = stride;
    out->data = data;
    out->data_offset = fh.bfOffBits;
    return 0;
}

int bmp_write(const char *path, const BmpImage *img) {
    if (!img || !img->data) return -1;

    FILE *f = fopen(path, "wb");
    if (!f) return -1;

    int width = img->width;
    int height = img->height;
    int abs_height = height < 0 ? -height : height;

    BmpFileHeader fh;
    BmpInfoHeader ih;

    uint32_t offBits = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);
    uint32_t imageSize = (uint32_t)(img->stride * abs_height);
    uint32_t fileSize = offBits + imageSize;

    fh.bfType = 0x4D42;
    fh.bfSize = fileSize;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = offBits;

    ih.biSize = 40;
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = imageSize;
    ih.biXPelsPerMeter = 0;
    ih.biYPelsPerMeter = 0;
    ih.biClrUsed = 0;
    ih.biClrImportant = 0;

    if (fwrite(&fh, sizeof(fh), 1, f) != 1) { fclose(f); return -1; }
    if (fwrite(&ih, sizeof(ih), 1, f) != 1) { fclose(f); return -1; }

    size_t n = fwrite(img->data, 1, imageSize, f);
    fclose(f);
    return n == imageSize ? 0 : -1;
}

void bmp_free(BmpImage *img) {
    if (!img) return;
    free(img->data);
    memset(img, 0, sizeof(*img));
}
