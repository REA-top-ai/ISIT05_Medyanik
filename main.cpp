#include <iostream>

#include "bmp.h"

int main(const int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "image.bmp";

    BMP bmp;

    if (!bmp.open(path)) {
        return 1;
    }

    bmp.print_headers();
    std::cout << std::endl;
    std::cout << "============ CORNER PIXELS ===========" << std::endl;
    bmp.print_pixel("Top left    ", 0,               0);
    bmp.print_pixel("Top right   ", bmp.width() - 1, 0);
    bmp.print_pixel("Bottom left ", 0,               bmp.height() - 1);
    bmp.print_pixel("Bottom right", bmp.width() - 1, bmp.height() - 1);
    std::cout << "======================================" << std::endl;

    return 0;
}
