#ifndef __IMAGE_H
#define __IMAGE_H

#include "color.h"
#include "lodepng.h"
#include <algorithm> // std::clamp
#include <iostream> // std::err, std::endl
#include <vector> // std::vector

class Image
{
public:
    Image();

    void reset();

    Color get_pixel(unsigned int x, unsigned int y);
    void set_pixel(Color color, unsigned int x, unsigned int y);
    std::size_t get_width();
    std::size_t get_height();
    double get_gamma();

    std::size_t load(const char* file_name);
    std::size_t save(const char* file_name);

private:
    std::vector<unsigned char> pixels;
    unsigned int width;
    unsigned int height;
    double gamma;
};

#endif
