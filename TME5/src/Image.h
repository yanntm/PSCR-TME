#pragma once

#include "Color.h"
#include <fstream>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace pr {

// Simple image class to hold pixels and export to PPM
class Image {
private:
    size_t width_;
    size_t height_;
    Color* pixels_;

public:
    Image(size_t width, size_t height) : width_(width), height_(height), pixels_(new Color[width * height]) {}

    ~Image() {
        delete[] pixels_;
    }

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    Color& pixel(size_t x, size_t y) {
        return pixels_[y * width_ + x];
    }

    const Color& pixel(size_t x, size_t y) const {
        return pixels_[y * width_ + x];
    }

    void exportToBMP(const char* path) const {

        // uses C-style file handling for simplicity
        // the void *,size_t API of fwrite is more adapted to write binary data than std::fstream
        // signature of fwrite: size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
        // ptr: pointer to the data to write
        // size: size of each element to write
        // count: number of elements to write
        // stream: file pointer 
        FILE* fp = fopen(path, "wb");
        if (!fp) {
            // Handle error if needed, e.g., throw or log
            return;
        }

        const size_t bytes_per_pixel = 3;
        const size_t row_size = ((bytes_per_pixel * width_ + 3) / 4) * 4;  // Pad to multiple of 4 bytes
        const size_t pixel_data_size = row_size * height_;
        const size_t file_size = 14 + 40 + pixel_data_size;  // File header + info header + pixels
        const size_t padding_size = row_size - (bytes_per_pixel * width_);
        const unsigned char padding[3] = {0, 0, 0};  // Max padding is 3 bytes, so static array

        // Packed structs for BMP headers to ensure no padding
        #pragma pack(push, 1)
        struct BMPFileHeader {
            char bm[2] = {'B', 'M'};
            uint32_t file_size;
            uint32_t reserved = 0;
            uint32_t data_offset = 54;
        };
        struct DIBHeader {
            uint32_t header_size = 40;
            int32_t width, height;
            uint16_t planes = 1, bpp = 24;
            uint32_t compression = 0, image_size;
            int32_t res_x = 0, res_y = 0;
            uint32_t colors = 0, important = 0;
        };
        #pragma pack(pop)

        // Compile-time asserts to ensure correct struct sizes (no padding)
        static_assert(sizeof(BMPFileHeader) == 14, "BMPFileHeader must be 14 bytes");
        static_assert(sizeof(DIBHeader) == 40, "DIBHeader must be 40 bytes");

        // Write headers
        BMPFileHeader fh;
        fh.file_size = static_cast<uint32_t>(file_size);
        fwrite(&fh, sizeof(fh), 1, fp);

        DIBHeader dib;
        dib.width = static_cast<int32_t>(width_);
        dib.height = static_cast<int32_t>(height_);
        dib.image_size = static_cast<uint32_t>(pixel_data_size);
        fwrite(&dib, sizeof(dib), 1, fp);

        // Pixel Data (BGR order, bottom-up rows)
        for (size_t y = height_; y > 0; --y) {
            size_t row_idx = y - 1;
            // Write the entire row of pixels in one go
            fwrite(&pixels_[row_idx * width_], sizeof(Color), width_, fp);
            if (padding_size > 0) {
                fwrite(padding, 1, padding_size, fp);
            }
        }

        fclose(fp);
    }
};

} /* namespace pr */