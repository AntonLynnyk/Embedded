#include "stm32f10x_conf.h"
#include "bmp.h"

#define UINT16_SIZE sizeof(uint16_t)
#define UINT32_SIZE sizeof(uint32_t)

#define UINT16_PTR_VALUE(x) ( *( (uint16_t*) (x) ) )
#define UINT32_PTR_VALUE(x) ( *( (uint32_t*) (x) ) )

static void ReadFileHeader(BMPFileHeader *fileHeaderP, uint8_t *bufferP);
static void ReadImageHeader(BMPImageHeader *imageHeaderP, uint8_t *bufferP);

void BMPReadInfoHeaders(BMPFileHeader *fileHeaderP, BMPImageHeader *imageHeaderP, uint8_t *bufferP)
{
    ReadFileHeader(fileHeaderP, bufferP);
    bufferP += BMP_FILE_HEADER_SIZE;

    ReadImageHeader(imageHeaderP, bufferP);
}

FUNC_RESULT BMPValidateImageParameters(BMPFileHeader *fileHeaderP, BMPImageHeader *imageHeaderP)
{
    if (fileHeaderP->bfType != BMP_IMAGE_SIGNATURE)
        return RES_ERROR;
    if (imageHeaderP->biWidth != BMP_IMAGE_WIDTH)
        return RES_ERROR;
    if (imageHeaderP->biHeight != BMP_IMAGE_HEIGHT)
        return RES_ERROR;
    if (imageHeaderP->biBitCount != BMP_IMAGE_BYTES_PER_COLOR)
        return RES_ERROR;
    if (imageHeaderP->biCompression != BMP_IMAGE_COMPRESSION)
        return RES_ERROR;

    return RES_OK;
}

static void ReadFileHeader(BMPFileHeader *fileHeaderP, uint8_t *bufferP)
{
    fileHeaderP->bfType = UINT16_PTR_VALUE(bufferP);
    bufferP += UINT16_SIZE;

    fileHeaderP->bfSize = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    fileHeaderP->bfReserved1 = UINT16_PTR_VALUE(bufferP);
    bufferP += UINT16_SIZE;

    fileHeaderP->bfReserver2 = UINT16_PTR_VALUE(bufferP);
    bufferP += UINT16_SIZE;

    fileHeaderP->bfOffBits = UINT32_PTR_VALUE(bufferP);
}

static void ReadImageHeader(BMPImageHeader *imageHeaderP, uint8_t *bufferP)
{
	imageHeaderP->biSize = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biWidth = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biHeight = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biPlanes = UINT16_PTR_VALUE(bufferP);
    bufferP += UINT16_SIZE;

    imageHeaderP->biBitCount = UINT16_PTR_VALUE(bufferP);
    bufferP += UINT16_SIZE;

    imageHeaderP->biCompression = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biSizeImage = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biXPelsPerMeter = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biYPelsPerMeter = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biClrUsed = UINT32_PTR_VALUE(bufferP);
    bufferP += UINT32_SIZE;

    imageHeaderP->biClrImportant = UINT32_PTR_VALUE(bufferP);
}
