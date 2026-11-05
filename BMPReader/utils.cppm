module;

#include <cstdint>

export module utils;

export int get_brightness(int red, int green, int blue);
export int extract_channel(std::uint32_t value, std::uint32_t mask);
