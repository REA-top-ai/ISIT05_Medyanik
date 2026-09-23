#include <iostream>
#include <fstream>

bool load_file_bytes(const char* path, unsigned char* headers, int size) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    file.read(reinterpret_cast<char*>(headers), size);
    return true;
}

int bytes_to_int16(unsigned char* bytes) {
    return bytes[0] + bytes[1] * 256;
}

int bytes_to_int32(unsigned char* bytes) {
    return bytes[0] + bytes[1] * 256 + bytes[2] * 256 * 256 + bytes[3] * 256 * 256 * 256;
}

int main(const int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "image.bmp";


    unsigned char headers[54];

    if (!load_file_bytes(path, headers, 54)) {
        std::cout << "Не удалось открыть файл: " << path << std::endl;
        return 1;
    }

    if (headers[0] != 'B' || headers[1] != 'M') {
        std::cout << "Это не BMP файл!" << std::endl;
        return 1;
    }

    int file_size   = bytes_to_int32(headers + 2); 
    int data_offset = bytes_to_int32(headers + 10); 

    int header_size = bytes_to_int32(headers + 14);
    int width       = bytes_to_int32(headers + 18); 
    int height      = bytes_to_int32(headers + 22); 
    int planes      = bytes_to_int16(headers + 26);
    int bpp         = bytes_to_int16(headers + 28);
    int compression = bytes_to_int32(headers + 30); 
    int image_size  = bytes_to_int32(headers + 34);
    int x_pixels_per_m   = bytes_to_int32(headers + 38);
    int y_pixels_per_m   = bytes_to_int32(headers + 42); 
    int colors_used       = bytes_to_int32(headers + 46);
    int colors_important  = bytes_to_int32(headers + 50); 
    std::cout << "2b headers.bitmap_signature : BM" << std::endl;
    std::cout << "4b headers.bitmap_file_size : " << file_size << std::endl;
    std::cout << "4b headers.bitmap_data_offset : " << data_offset << std::endl;
    std::cout << "4b headers.bitmap_info_header_size : " << header_size << std::endl;
    std::cout << "4b headers.bitmap_width : " << width << std::endl;
    std::cout << "4b headers.bitmap_height : " << height << std::endl;
    std::cout << "2b headers.bitmap_planes : " << planes << std::endl;
    std::cout << "2b headers.bitmap_bits_per_pixel : " << bpp << std::endl;
    std::cout << "4b headers.bitmap_compression : " << compression << std::endl;
    std::cout << "4b headers.bitmap_image_size : " << image_size << std::endl;
    std::cout << "4b headers.bitmap_XpixelsPerM : " << x_pixels_per_m << std::endl;
    std::cout << "4b headers.bitmap_YpixelsPerM : " << y_pixels_per_m << std::endl;
    std::cout << "4b headers.bitmap_ColorsUsed : " << colors_used << std::endl;
    std::cout << "4b headers.bitmap_ColorsImportant : " << colors_important << std::endl;

    return 0;
}