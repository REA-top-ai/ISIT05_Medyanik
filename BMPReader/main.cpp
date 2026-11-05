#include <iostream>
#include <memory>

import image;
import bmp;
import errors;

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "lena.bmp";

    std::unique_ptr<Image> image = std::make_unique<BMP>();

    try {
        image->open(path);
        image->print_headers();

        std::cout << '\n';
        std::cout << "============ CORNER PIXELS ===========\n";
        image->print_pixel("Top left     ", 0, 0);
        image->print_pixel("Top right    ", image->width() - 1, 0);
        image->print_pixel("Bottom left  ", 0, image->height() - 1);
        image->print_pixel("Bottom right ", image->width() - 1, image->height() - 1);
        std::cout << "======================================\n";
    } catch (const FileOpenError& e) {
        std::cerr << "[Ошибка файла] " << e.what() << '\n';
        return 1;
    } catch (const FormatError& e) {
        std::cerr << "[Ошибка формата] " << e.what() << '\n';
        return 1;
    } catch (const ImageError& e) {
        std::cerr << "[Ошибка изображения] " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[Непредвиденная ошибка] " << e.what() << '\n';
        return 1;
    }

    return 0;
}
