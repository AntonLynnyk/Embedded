#ifndef BMP_H
#define BMP_H

#define BMP_FILE_HEADER_SIZE 14
#define BMP_IMAGE_HEADER_SIZE 40

#define BMP_IMAGE_WIDTH 240
#define BMP_IMAGE_HEIGHT 320
#define BMP_IMAGE_BYTES_PER_COLOR 24
#define BMP_IMAGE_COMPRESSION 0
#define BMP_IMAGE_SIGNATURE 0x4D42 // "BM"

typedef struct
{
    uint16_t bfType;        // The characters "BM"
    uint32_t bfSize;        // The size of the file in bytes
    uint16_t bfReserved1;   // Unused - must be zero
    uint16_t bfReserver2;   // Unused - must be zero
    uint32_t bfOffBits;      // Offset to start of Pixel Data
} BMPFileHeader;

typedef struct
{
    uint32_t biSize;	      // Header Size - Must be at least 40
    uint32_t biWidth;	      // Image width in pixels
    uint32_t biHeight;	      // Image height in pixels
    uint16_t biPlanes;		  // Must be 1
    uint16_t biBitCount;	  // Bits per pixel - 1, 4, 8, 16, 24, or 32
    uint32_t biCompression;	  // Compression type (0 = uncompressed)
    uint32_t biSizeImage;	  // Image Size - may be zero for uncompressed images
    uint32_t biXPelsPerMeter; // Preferred resolution in pixels per meter
    uint32_t biYPelsPerMeter; // Preferred resolution in pixels per meter
    uint32_t biClrUsed;	      // Number Color Map entries that are actually used
    uint32_t biClrImportant;   // Number of significant colors
} BMPImageHeader;

typedef enum
{
	Uncompressed,
	RLE_8,           // Usable only with 8-bit images
	RLE_4,           // Usable only with 4-bit images
	Bitfields       // Used - and required - only with 16- and 32-bit images
} BMPImageCompression;

typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} FUNC_RESULT;

typedef enum
{
    FUCK_OFF
} DONT;

void BMPReadInfoHeaders(BMPFileHeader *fileHeaderP, BMPImageHeader *imageHeaderP, uint8_t *bufferP);
FUNC_RESULT BMPValidateImageParameters(BMPFileHeader *fileHeaderP, BMPImageHeader *imageHeaderP);

#endif
