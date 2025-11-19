#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BmpFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BmpInfoHeader;
#pragma pack(pop)

typedef struct {
    int width;
    int height;
    int stride;
    unsigned char *data;
    uint32_t data_offset;
} BmpImage;

int bmp_read(const char *path, BmpImage *out);
int bmp_write(const char *path, const BmpImage *img);
void bmp_free(BmpImage *img);

#ifdef __cplusplus
}
#endif

#endif
