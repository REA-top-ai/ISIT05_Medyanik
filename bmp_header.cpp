#include <iostream>
#include <fstream>
#include <string>

struct BmpHeaders {
    int file_size;
    int data_offset;
    int header_size;
    int width;
    int height;
    int planes;
    int bpp;
    int compression;
    int image_size;
    int x_pixels_per_m;
    int y_pixels_per_m;
    int colors_used;
    int colors_important;
    int abs_height;
};

class BmpImage {
public:
    bool load(const char* file_path) {
        path = file_path;

        unsigned char raw[54];
        if (!load_file_bytes(path.c_str(), raw, 54)) {
            std::cout << "Не удалось открыть файл: " << path << std::endl;
            return false;
        }
        if (raw[0] != 'B' || raw[1] != 'M') {
            std::cout << "Это не BMP файл!" << std::endl;
            return false;
        }

        h.file_size        = bytes_to_int32(raw + 2);
        h.data_offset      = bytes_to_int32(raw + 10);
        h.header_size      = bytes_to_int32(raw + 14);
        h.width            = bytes_to_int32(raw + 18);
        h.height           = bytes_to_int32(raw + 22);
        h.planes           = bytes_to_int16(raw + 26);
        h.bpp              = bytes_to_int16(raw + 28);
        h.compression      = bytes_to_int32(raw + 30);
        h.image_size       = bytes_to_int32(raw + 34);
        h.x_pixels_per_m   = bytes_to_int32(raw + 38);
        h.y_pixels_per_m   = bytes_to_int32(raw + 42);
        h.colors_used      = bytes_to_int32(raw + 46);
        h.colors_important = bytes_to_int32(raw + 50);
        h.abs_height = h.height;
        if (h.height < 0) {
            h.abs_height = -h.height;
        }

        return true;
    }

    int get_width() const {
        return h.width;
    }

    int get_height() const {
        return h.abs_height;
    }

    void print_headers() const {
        std::cout << "======================================" << std::endl;
        std::cout << "            BMP HEADERS               " << std::endl;
        std::cout << "======================================" << std::endl;
        std::cout << "Signature : BM" << std::endl;
        std::cout << "File size : " << h.file_size << std::endl;
        std::cout << "Data offset : " << h.data_offset << std::endl;
        std::cout << "Header size : " << h.header_size << std::endl;
        std::cout << "Width : " << h.width << std::endl;
        std::cout << "Height : " << h.height << std::endl;
        std::cout << "Planes : " << h.planes << std::endl;
        std::cout << "Bits per pixel : " << h.bpp << std::endl;
        std::cout << "Compression : " << h.compression << std::endl;
        std::cout << "Image size : " << h.image_size << std::endl;
        std::cout << "X pixels per M : " << h.x_pixels_per_m << std::endl;
        std::cout << "Y pixels per M : " << h.y_pixels_per_m << std::endl;
        std::cout << "Colors used : " << h.colors_used << std::endl;
        std::cout << "Colors important : " << h.colors_important << std::endl;
        std::cout << "======================================" << std::endl;
    }

    void print_pixel(const char* name, int x, int y) const {
        int red, green, blue;
        std::cout << name << " (" << x << ", " << y << ") : ";
        if (!read_pixel(x, y, red, green, blue)) {
            std::cout << "Не удалось прочитать пиксель" << std::endl;
            return;
        }
        int brightness = get_brightness(red, green, blue);

        std::cout << "R=" << red << " G=" << green << " B=" << blue
                  << " | brightness = " << brightness << std::endl;
    }

private:
    std::string path;
    BmpHeaders h{};

    static bool load_file_bytes(const char* file_path, unsigned char* headers, int size) {
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return false;
        }
        file.read(reinterpret_cast<char*>(headers), size);
        return true;
    }

    static int bytes_to_int16(unsigned char* bytes) {
        return bytes[0] + bytes[1] * 256;
    }

    static int bytes_to_int32(unsigned char* bytes) {
        return bytes[0] + bytes[1] * 256 + bytes[2] * 256 * 256 + bytes[3] * 256 * 256 * 256;
    }

    bool read_pixel(int x, int y, int& red, int& green, int& blue) const {
        if (h.bpp != 24 && h.bpp != 32) {
            return false;
        }
        if (x < 0 || x >= h.width || y < 0 || y >= h.abs_height) {
            return false;
        }
        int row;
        if (h.height > 0) {
            row = h.abs_height - 1 - y;
        } else {
            row = y;
        }

        int bytes_per_pixel = h.bpp / 8;
        int row_size = (h.width * h.bpp + 31) / 32 * 4;

        int position = h.data_offset + row * row_size + x * bytes_per_pixel;

        std::ifstream file(path, std::ios::binary);
        file.seekg(position);

        unsigned char pixel[4];
        file.read(reinterpret_cast<char*>(pixel), bytes_per_pixel);

        blue  = pixel[0];
        green = pixel[1];
        red   = pixel[2];

        return true;
    }

    static int get_brightness(int red, int green, int blue) {
        return 0.299 * red + 0.587 * green + 0.114 * blue;
    }
};

int main(const int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "image.bmp";

    BmpImage image;

    if (!image.load(path)) {
        return 1;
    }

    image.print_headers();
    std::cout << std::endl;
    std::cout << "============ CORNER PIXELS ===========" << std::endl;
    image.print_pixel("Top left    ", 0,                     0);
    image.print_pixel("Top right   ", image.get_width() - 1,  0);
    image.print_pixel("Bottom left ", 0,                     image.get_height() - 1);
    image.print_pixel("Bottom right", image.get_width() - 1,  image.get_height() - 1);
    std::cout << "======================================" << std::endl;

    return 0;
}