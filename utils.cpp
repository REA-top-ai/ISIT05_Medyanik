#include "utils.h"

#include <fstream>

bool load_file_bytes(const char* path, unsigned char* buffer, int size) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    file.read(reinterpret_cast<char*>(buffer), size);
    return true;
}

int bytes_to_int16(const unsigned char* bytes) {
    return bytes[0] + bytes[1] * 256;
}

int bytes_to_int32(const unsigned char* bytes) {
    return bytes[0] + bytes[1] * 256 + bytes[2] * 256 * 256 + bytes[3] * 256 * 256 * 256;
}

int get_brightness(int red, int green, int blue) {
    return 0.299 * red + 0.587 * green + 0.114 * blue;
}
