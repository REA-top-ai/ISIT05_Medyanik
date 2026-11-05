export module image;

export class Image {
public:
    virtual ~Image() = default;

    virtual void open(const char* path) = 0;
    virtual void print_headers() const = 0;
    virtual void read_pixel(int x, int y, int& red, int& green, int& blue) const = 0;
    virtual void print_pixel(const char* name, int x, int y) const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

protected:
    Image() = default;
    Image(const Image&) = default;
    Image(Image&&) = default;
    Image& operator=(const Image&) = default;
    Image& operator=(Image&&) = default;
};
