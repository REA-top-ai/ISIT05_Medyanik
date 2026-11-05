module;

#include <stdexcept>

export module errors;

export class ImageError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

export class FileOpenError : public ImageError {
public:
    using ImageError::ImageError;
};

export class FormatError : public ImageError {
public:
    using ImageError::ImageError;
};

export class PixelError : public ImageError {
public:
    using ImageError::ImageError;
};
