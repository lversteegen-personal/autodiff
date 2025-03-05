// Taken from https://github.com/yunhao-qian/save-bmp/blob/master/save_bmp.h

#ifndef SAVE_BMP_H
#define SAVE_BMP_H

#include <stdint.h>

namespace bitmap
{

    enum save_bmp_result
    {
        SAVE_BMP_SUCCESS,
        SAVE_BMP_SIZE_IS_ZERO,
        SAVE_BMP_SIZE_TOO_BIG,
        SAVE_BMP_CANT_OPEN_FILE,
        SAVE_BMP_WRITE_ERROR
    };

#ifdef __cplusplus
    extern "C"
    {
#endif // __cplusplus

        enum save_bmp_result save_bmp(const char *filename,
                                      uint32_t width, uint32_t height,
                                      const uint8_t *image);

        const char *save_bmp_str_result(enum save_bmp_result result);

#ifdef __cplusplus
    } // extern "C"
#endif // __cplusplus

#include <errno.h>
#include <stdio.h>
#include <string.h>

    enum save_bmp_result save_bmp(const char *filename,
                                  uint32_t width, uint32_t height,
                                  const uint8_t *image)
    {
        if (width == 0 || height == 0)
            return SAVE_BMP_SIZE_IS_ZERO;

        if (width > 0x55555542 || height > 0x3ffffff2)
            return SAVE_BMP_SIZE_TOO_BIG;
        const uint32_t padding = width % 4;
        const uint64_t bf_size = ((uint64_t)width * 3 + padding) * height + 54;
        if (bf_size > 0xffffffff)
            return SAVE_BMP_SIZE_TOO_BIG;

        FILE *const stream = fopen(filename, "wb");
        if (stream == NULL)
            return SAVE_BMP_CANT_OPEN_FILE;

        {
            uint8_t header[54] = {'B', 'M'};
            const uint32_t bi_height = ~height + 1;

#define SAVE_BMP_U16(offset, value)               \
    do                                            \
    {                                             \
        header[offset + 0] = 0xff & (value >> 0); \
        header[offset + 1] = 0xff & (value >> 8); \
    } while (0)

#define SAVE_BMP_U32(offset, value)                \
    do                                             \
    {                                              \
        header[offset + 0] = 0xff & (value >> 0);  \
        header[offset + 1] = 0xff & (value >> 8);  \
        header[offset + 2] = 0xff & (value >> 16); \
        header[offset + 3] = 0xff & (value >> 24); \
    } while (0)

            SAVE_BMP_U32(2, bf_size);    // bf_size
            SAVE_BMP_U32(10, 54);        // bf_off_bits
            SAVE_BMP_U32(14, 40);        // bi_size
            SAVE_BMP_U32(18, width);     // bi_width
            SAVE_BMP_U32(22, bi_height); // bi_height
            SAVE_BMP_U16(26, 1);         // bi_planes
            SAVE_BMP_U16(28, 24);        // bi_bit_count
            SAVE_BMP_U32(38, 2835);      // bi_x_pels_per_meter
            SAVE_BMP_U32(42, 2835);      // bi_y_pels_per_meter

#undef SAVE_BMP_U16
#undef SAVE_BMP_U32

            if (fwrite(header, 54, 1, stream) == 0)
                goto write_error;
        }

        for (uint32_t row = 0; row < height; ++row)
        {
            for (uint32_t column = 0; column < width; ++column, image += 3)
                if (fputc(image[2], stream) == EOF ||
                    fputc(image[1], stream) == EOF ||
                    fputc(image[0], stream) == EOF)
                    goto write_error;

            for (uint32_t i = 0; i < padding; ++i)
                if (fputc(0, stream) == EOF)
                    goto write_error;
        }

        fclose(stream);
        return SAVE_BMP_SUCCESS;

    write_error:
        fclose(stream);
        return SAVE_BMP_WRITE_ERROR;
    }

    const char *save_bmp_str_result(enum save_bmp_result result)
    {
        switch (result)
        {
        case SAVE_BMP_SUCCESS:
            return "Success";
        case SAVE_BMP_SIZE_IS_ZERO:
            return "Error saving image: image size is zero";
        case SAVE_BMP_SIZE_TOO_BIG:
            return "Error saving image: image size is too big";
        case SAVE_BMP_CANT_OPEN_FILE:
        case SAVE_BMP_WRITE_ERROR:
            return strerror(errno);
        default:
            return "Unknown result code";
        }
    }
} // namespace bitmap

#endif // SAVE_BMP_IMPLEMENT