#pragma once

bool load_file_bytes(const char* path, unsigned char* buffer, int size);

int bytes_to_int16(const unsigned char* bytes);
int bytes_to_int32(const unsigned char* bytes);

int get_brightness(int red, int green, int blue);
