module;

export module utils;

export bool load_file_bytes(const char* path, unsigned char* buffer, int size);
export int bytes_to_int16(const unsigned char* bytes);
export int bytes_to_int32(const unsigned char* bytes);
export int get_brightness(int red, int green, int blue);
export int extract_channel(unsigned int value, unsigned int mask);
