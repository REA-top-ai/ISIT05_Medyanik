module;

#include <array>
#include <cstdint>
#include <string>

export module bmp;

import image;

#pragma pack(push, 1)
export struct BMPHeader {
    char signature[2];
    std::uint32_t file_size;
    std::uint16_t reserved1;
    std::uint16_t reserved2;
    std::uint32_t data_offset;

    std::uint32_t dib_header_size;
    std::int32_t width;
    std::int32_t height;
    std::uint16_t planes;
    std::uint16_t bits_per_pixel;
    std::uint32_t compression;
    std::uint32_t image_size;
    std::int32_t x_pixels_per_meter;
    std::int32_t y_pixels_per_meter;
    std::uint32_t colors_used;
    std::uint32_t colors_important;
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
    std::array<std::uint32_t, 3> color_masks_{};
};
