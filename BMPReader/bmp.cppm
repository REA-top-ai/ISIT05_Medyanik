module;

#include <array>
#include <string>

export module bmp;

import image;

static_assert(sizeof(short) == 2 && sizeof(int) == 4,
              "BMPHeader рассчитан на short = 2 байта и int = 4 байта");

#pragma pack(push, 1)
export struct BMPHeader {
    char signature[2];
    unsigned int file_size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int data_offset;

    unsigned int dib_header_size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bits_per_pixel;
    unsigned int compression;
    unsigned int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    unsigned int colors_used;
    unsigned int colors_important;
};
#pragma pack(pop)

static_assert(sizeof(BMPHeader) == 54);

export class BMP : public Image {
public:
    BMP();
    explicit BMP(const char* path);
    BMP(const BMP& other);
    BMP(BMP&& other) noexcept;
    ~BMP() override;

    void open(const char* path) override;
    void print_headers() const override;
    void read_pixel(int x, int y, int& red, int& green, int& blue) const override;
    void print_pixel(const char* name, int x, int y) const override;

    int width() const override { return header_.width; }
    int height() const override;

private:
    std::string path_;
    BMPHeader header_{};
    std::array<unsigned int, 3> color_masks_{};
};
