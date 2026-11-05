module;

#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

module bmp;

import utils;
import errors;

namespace {
constexpr unsigned int BI_RGB = 0;
constexpr unsigned int BI_BITFIELDS = 3;
constexpr unsigned int BI_ALPHABITFIELDS = 6;

constexpr unsigned int MASK_16_BIT_LIMIT = 0xFFFF0000U;

bool masks_are_valid(const std::array<unsigned int, 3>& masks) {
    if (masks[0] == 0 || masks[1] == 0 || masks[2] == 0) {
        return false;
    }

    if (((masks[0] | masks[1] | masks[2]) & MASK_16_BIT_LIMIT) != 0) {
        return false;
    }

    return (masks[0] & masks[1]) == 0 &&
           (masks[0] & masks[2]) == 0 &&
           (masks[1] & masks[2]) == 0;
}
}

BMP::BMP() = default;
BMP::BMP(const char* path) { open(path); }
BMP::BMP(const BMP& other) = default;
BMP::BMP(BMP&& other) noexcept = default;
BMP::~BMP() = default;

int BMP::height() const {
    return header_.height < 0 ? -header_.height : header_.height;
}

void BMP::open(const char* path) {
    if (path == nullptr || *path == '\0') {
        throw FileOpenError("Не указано имя файла");
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw FileOpenError(std::string("Не удалось открыть файл: ") + path);
    }

    BMPHeader new_header{};
    file.read(reinterpret_cast<char*>(&new_header), sizeof(new_header));

    if (file.gcount() != static_cast<std::streamsize>(sizeof(new_header))) {
        throw FormatError("Файл слишком мал: не удалось прочитать полный BMP-заголовок");
    }

    if (new_header.signature[0] != 'B' || new_header.signature[1] != 'M') {
        throw FormatError("Это не BMP-файл");
    }

    if (new_header.dib_header_size < 40) {
        throw FormatError("Поддерживаются BMP-заголовки DIB размером не менее 40 байт");
    }

    const unsigned long long minimum_pixel_offset =
        14ULL + static_cast<unsigned long long>(new_header.dib_header_size);
    if (new_header.data_offset < minimum_pixel_offset) {
        throw FormatError("Некорректное смещение данных пикселей");
    }

    if (new_header.width <= 0 || new_header.height == 0 ||
        new_header.height == std::numeric_limits<int>::min()) {
        throw FormatError("Некорректные размеры изображения");
    }

    if (new_header.planes != 1) {
        throw FormatError("Некорректное количество цветовых плоскостей BMP");
    }

    const unsigned short bpp = new_header.bits_per_pixel;
    const unsigned int compression = new_header.compression;

    if (bpp != 16 && bpp != 24 && bpp != 32) {
        throw FormatError("Поддерживаются только BMP с глубиной цвета 16, 24 или 32 бита");
    }

    if (bpp == 16) {
        if (compression != BI_RGB && compression != BI_BITFIELDS &&
            compression != BI_ALPHABITFIELDS) {
            throw FormatError("Неподдерживаемый тип сжатия 16-битного BMP");
        }
    } else if (compression != BI_RGB) {
        throw FormatError("Для 24- и 32-битного BMP поддерживается только BI_RGB");
    }

    std::array<unsigned int, 3> new_masks{};

    if (bpp == 16 && compression == BI_RGB) {
        new_masks = {0x7C00U, 0x03E0U, 0x001FU};
    } else if (bpp == 16 &&
               (compression == BI_BITFIELDS || compression == BI_ALPHABITFIELDS)) {
        constexpr long long masks_offset = 14 + 40;

        if (new_header.dib_header_size == 40) {
            const unsigned long long required =
                static_cast<unsigned long long>(masks_offset) + 12ULL;
            if (new_header.data_offset < required) {
                throw FormatError("В BMP отсутствуют RGB-маски перед пиксельными данными");
            }
        } else if (new_header.dib_header_size < 52) {
            throw FormatError("DIB-заголовок недостаточен для встроенных RGB-масок");
        }

        file.seekg(masks_offset);
        if (!file) {
            throw FormatError("Не удалось перейти к RGB-маскам");
        }

        file.read(reinterpret_cast<char*>(new_masks.data()), 12);
        if (file.gcount() != 12) {
            throw FormatError("Файл повреждён: не удалось прочитать RGB-маски");
        }

        if (!masks_are_valid(new_masks)) {
            throw FormatError("Некорректные RGB-маски BMP");
        }
    }

    header_ = new_header;
    color_masks_ = new_masks;
    path_ = path;
}

void BMP::print_headers() const {
    const BMPHeader* h = &header_;

    std::cout << "======================================\n";
    std::cout << "              BMP HEADERS             \n";
    std::cout << "======================================\n";
    std::cout << "Signature        : " << h->signature[0] << h->signature[1] << '\n';
    std::cout << "File size        : " << h->file_size << '\n';
    std::cout << "Data offset      : " << h->data_offset << '\n';
    std::cout << "Header size      : " << h->dib_header_size << '\n';
    std::cout << "Width            : " << h->width << '\n';
    std::cout << "Height           : " << h->height << '\n';
    std::cout << "Planes           : " << h->planes << '\n';
    std::cout << "Bits per pixel   : " << h->bits_per_pixel << '\n';
    std::cout << "Compression      : " << h->compression << '\n';
    std::cout << "Image size       : " << h->image_size << '\n';
    std::cout << "X pixels per M   : " << h->x_pixels_per_meter << '\n';
    std::cout << "Y pixels per M   : " << h->y_pixels_per_meter << '\n';
    std::cout << "Colors used      : " << h->colors_used << '\n';
    std::cout << "Colors important : " << h->colors_important << '\n';

    if (h->bits_per_pixel == 16) {
        std::cout << "Red mask         : 0x" << std::hex << color_masks_[0] << std::dec << '\n';
        std::cout << "Green mask       : 0x" << std::hex << color_masks_[1] << std::dec << '\n';
        std::cout << "Blue mask        : 0x" << std::hex << color_masks_[2] << std::dec << '\n';
    }

    std::cout << "======================================\n";
}

void BMP::read_pixel(int x, int y, int& red, int& green, int& blue) const {
    if (path_.empty()) {
        throw PixelError("Изображение не открыто");
    }

    const int image_width = header_.width;
    const int image_height = height();
    const unsigned short bpp = header_.bits_per_pixel;

    if (x < 0 || x >= image_width || y < 0 || y >= image_height) {
        throw PixelError("Координаты (" + std::to_string(x) + ", " + std::to_string(y) +
                         ") вне изображения " + std::to_string(image_width) + "x" +
                         std::to_string(image_height));
    }

    const unsigned long long row =
        header_.height > 0
            ? static_cast<unsigned long long>(image_height - 1 - y)
            : static_cast<unsigned long long>(y);

    const unsigned long long bytes_per_pixel = bpp / 8;
    const unsigned long long row_size =
        (static_cast<unsigned long long>(image_width) * bpp + 31ULL) / 32ULL * 4ULL;
    const unsigned long long position =
        static_cast<unsigned long long>(header_.data_offset) +
        row * row_size +
        static_cast<unsigned long long>(x) * bytes_per_pixel;

    if (position > static_cast<unsigned long long>(
                       std::numeric_limits<long long>::max())) {
        throw PixelError("Слишком большое смещение пикселя");
    }

    std::ifstream file(path_, std::ios::binary);
    if (!file) {
        throw FileOpenError("Не удалось повторно открыть файл: " + path_);
    }

    file.seekg(static_cast<long long>(position));
    if (!file) {
        throw PixelError("Не удалось перейти к данным пикселя");
    }

    unsigned char pixel[4]{};
    file.read(reinterpret_cast<char*>(pixel),
              static_cast<std::streamsize>(bytes_per_pixel));

    if (file.gcount() != static_cast<std::streamsize>(bytes_per_pixel)) {
        throw PixelError("Файл повреждён или обрезан: пиксель (" +
                         std::to_string(x) + ", " + std::to_string(y) +
                         ") находится за пределами данных");
    }

    if (bpp == 16) {
        const unsigned int value =
            static_cast<unsigned int>(pixel[0]) |
            (static_cast<unsigned int>(pixel[1]) << 8U);

        red = extract_channel(value, color_masks_[0]);
        green = extract_channel(value, color_masks_[1]);
        blue = extract_channel(value, color_masks_[2]);
        return;
    }

    blue = pixel[0];
    green = pixel[1];
    red = pixel[2];
}

void BMP::print_pixel(const char* name, int x, int y) const {
    int red = 0;
    int green = 0;
    int blue = 0;

    std::cout << name << " (" << x << ", " << y << ") : ";

    try {
        read_pixel(x, y, red, green, blue);
    } catch (const ImageError& e) {
        std::cout << "не удалось прочитать пиксель (" << e.what() << ")\n";
        return;
    }

    const int brightness = get_brightness(red, green, blue);
    std::cout << "R=" << red << " G=" << green << " B=" << blue
              << " | brightness = " << brightness << '\n';
}
