module utils;

int get_brightness(int red, int green, int blue) {
    return static_cast<int>(0.299 * red + 0.587 * green + 0.114 * blue);
}

int extract_channel(unsigned int value, unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    unsigned int shift = 0;
    while (((mask >> shift) & 1U) == 0U) {
        ++shift;
    }

    const unsigned int max_value = mask >> shift;
    const unsigned int channel = (value & mask) >> shift;

    return static_cast<int>((static_cast<unsigned long long>(channel) * 255U) / max_value);
}
