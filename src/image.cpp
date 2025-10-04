#include "image.h"

Image::Image()
{
    pixels.resize(0);
    width = 0;
    height = 0;
    gamma = 0.0;
    return;
}

void Image::reset()
{
    pixels.resize(0);
    width = 0;
    height = 0;
    gamma = 0.0;
    return;
}

Color Image::get_pixel(unsigned int x, unsigned int y)
{
    const int index_start = Color::NUM_BYTES * width * y + Color::NUM_BYTES * x;
    return Color(
        pixels[index_start + Color::INDEX_R],
        pixels[index_start + Color::INDEX_G],
        pixels[index_start + Color::INDEX_B],
        pixels[index_start + Color::INDEX_A]
    );
}

void Image::set_pixel(Color color, unsigned int x, unsigned int y)
{
    const int index_start = Color::NUM_BYTES * width * y + Color::NUM_BYTES * x;
    pixels[index_start + Color::INDEX_R] = color.r;
    pixels[index_start + Color::INDEX_G] = color.g;
    pixels[index_start + Color::INDEX_B] = color.b;
    pixels[index_start + Color::INDEX_A] = color.a;
    return;
}

std::size_t Image::get_width()
{
    return width;
}

std::size_t Image::get_height()
{
    return height;
}

double Image::get_gamma()
{
    return gamma;
}

std::size_t Image::load(const char* file_name)
{
    reset();
    
    std::vector<unsigned char> png_buffer;
    lodepng::State state;

    std::size_t error = lodepng::load_file(png_buffer, file_name);

    if(!error)
    {
        error = lodepng::decode(pixels, width, height, state, png_buffer);
    }

    if(error)
    {
        std::cerr << "error: load" << file_name << " - " << error << ": " << lodepng_error_text(error) << std::endl;
        return error;
    }

    if(state.info_png.gama_defined)
    {
        gamma = 100000.0 / static_cast<double>(state.info_png.gama_gamma);
    }

    return error;
}

std::size_t Image::save(const char* file_name)
{
    std::size_t error = lodepng::encode(file_name, pixels, width, height);

    if(error)
    {
        std::cerr << "error: save - " << error << ": "<< lodepng_error_text(error) << std::endl;
    }

    return error;
}
