#pragma once

#include <string>

class BMP {
public:
    bool open(const char* path);
    void print_headers() const;
    bool read_pixel(int x, int y, int& red, int& green, int& blue) const;
    void print_pixel(const char* name, int x, int y) const;
    int width() const { return width_; }
    int height() const { return abs_height_; }

private:
    std::string path_;
    int file_size_ = 0;
    int data_offset_ = 0;
    int header_size_ = 0;
    int width_ = 0;
    int height_ = 0;
    int planes_ = 0;
    int bpp_ = 0;
    int compression_ = 0;
    int image_size_ = 0;
    int x_pixels_per_m_ = 0;
    int y_pixels_per_m_ = 0;
    int colors_used_ = 0;
    int colors_important_ = 0;
    int abs_height_ = 0;
};
