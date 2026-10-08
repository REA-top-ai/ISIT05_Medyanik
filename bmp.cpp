#include "bmp.h"

#include <fstream>
#include <iostream>

#include "utils.h"

bool BMP::open(const char* path) {
    unsigned char raw[54];
    if (!load_file_bytes(path, raw, 54)) {
        std::cout << "Не удалось открыть файл: " << path << std::endl;
        return false;
    }
    if (raw[0] != 'B' || raw[1] != 'M') {
        std::cout << "Это не BMP файл!" << std::endl;
        return false;
    }

    path_              = path;
    file_size_         = bytes_to_int32(raw + 2);
    data_offset_       = bytes_to_int32(raw + 10);
    header_size_       = bytes_to_int32(raw + 14);
    width_             = bytes_to_int32(raw + 18);
    height_            = bytes_to_int32(raw + 22);
    planes_            = bytes_to_int16(raw + 26);
    bpp_               = bytes_to_int16(raw + 28);
    compression_       = bytes_to_int32(raw + 30);
    image_size_        = bytes_to_int32(raw + 34);
    x_pixels_per_m_    = bytes_to_int32(raw + 38);
    y_pixels_per_m_    = bytes_to_int32(raw + 42);
    colors_used_       = bytes_to_int32(raw + 46);
    colors_important_  = bytes_to_int32(raw + 50);
    abs_height_ = height_;
    if (height_ < 0) {
        abs_height_ = -height_;
    }

    return true;
}

void BMP::print_headers() const {
    std::cout << "======================================" << std::endl;
    std::cout << "            BMP HEADERS               " << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Signature : BM" << std::endl;
    std::cout << "File size : " << file_size_ << std::endl;
    std::cout << "Data offset : " << data_offset_ << std::endl;
    std::cout << "Header size : " << header_size_ << std::endl;
    std::cout << "Width : " << width_ << std::endl;
    std::cout << "Height : " << height_ << std::endl;
    std::cout << "Planes : " << planes_ << std::endl;
    std::cout << "Bits per pixel : " << bpp_ << std::endl;
    std::cout << "Compression : " << compression_ << std::endl;
    std::cout << "Image size : " << image_size_ << std::endl;
    std::cout << "X pixels per M : " << x_pixels_per_m_ << std::endl;
    std::cout << "Y pixels per M : " << y_pixels_per_m_ << std::endl;
    std::cout << "Colors used : " << colors_used_ << std::endl;
    std::cout << "Colors important : " << colors_important_ << std::endl;
    std::cout << "======================================" << std::endl;
}

bool BMP::read_pixel(int x, int y, int& red, int& green, int& blue) const {
    if (bpp_ != 24 && bpp_ != 32) {
        return false;
    }

    if (x < 0 || x >= width_ || y < 0 || y >= abs_height_) {
        return false;
    }
    int row;
    if (height_ > 0) {
        row = abs_height_ - 1 - y;
    } else {
        row = y;
    }

    int bytes_per_pixel = bpp_ / 8;
    int row_size = (width_ * bpp_ + 31) / 32 * 4;

    int position = data_offset_ + row * row_size + x * bytes_per_pixel;

    std::ifstream file(path_, std::ios::binary);
    file.seekg(position);

    unsigned char pixel[4];
    file.read(reinterpret_cast<char*>(pixel), bytes_per_pixel);

    blue  = pixel[0];
    green = pixel[1];
    red   = pixel[2];

    return true;
}

void BMP::print_pixel(const char* name, int x, int y) const {
    int red, green, blue;

    std::cout << name << " (" << x << ", " << y << ") : ";

    if (!read_pixel(x, y, red, green, blue)) {
        std::cout << "не удалось прочитать пиксель" << std::endl;
        return;
    }

    int brightness = get_brightness(red, green, blue);

    std::cout << "R=" << red << " G=" << green << " B=" << blue
              << " | brightness = " << brightness << std::endl;
}
