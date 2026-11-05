# BMPReader

Чтение заголовков и угловых пикселей BMP (16 / 24 / 32 бита). C++20, модули (.cppm), CMake.

## Структура
- `utils.cppm` / `utils.cpp` — модуль `utils` (вспомогательные функции)
- `bmp.cppm` / `bmp.cpp` — модуль `bmp` (класс `BMP`)
- `main.cpp` — точка входа
- `CMakeLists.txt` — сборка

## Требования
CMake >= 3.28, Ninja (или Visual Studio), компилятор: GCC 14+, Clang 16+ или MSVC 2022.

## Сборка и запуск
    cmake -S . -B build -G Ninja
    cmake --build build
    ./build/bmp_reader lena.bmp
