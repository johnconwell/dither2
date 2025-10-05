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

std::vector<Color> Image::get_color_range()
{
    std::vector<Color> color_range = {
        Color(Color::CHANNEL_MAX, Color::CHANNEL_MAX, Color::CHANNEL_MAX, Color::CHANNEL_MAX),
        Color(0, 0, 0, Color::CHANNEL_MAX)
    };
    const std::size_t INDEX_COLOR_MIN = 0;
    const std::size_t INDEX_COLOR_MAX = 1;

    for(std::size_t y = 0; y < height; y++)
    {
        for(std::size_t x = 0; x < width; x++)
        {
            const Color color = get_pixel(x, y);

            if(color.r < color_range[INDEX_COLOR_MIN].r)
            {
                color_range[INDEX_COLOR_MIN].r = color.r;
            }
            else if(color.r > color_range[INDEX_COLOR_MAX].r)
            {
                color_range[INDEX_COLOR_MAX].r = color.r;
            }

            if(color.g < color_range[INDEX_COLOR_MIN].g)
            {
                color_range[INDEX_COLOR_MIN].g = color.g;
            }
            else if(color.g > color_range[INDEX_COLOR_MAX].g)
            {
                color_range[INDEX_COLOR_MAX].g = color.g;
            }

            if(color.b < color_range[INDEX_COLOR_MIN].b)
            {
                color_range[INDEX_COLOR_MIN].b = color.b;
            }
            else if(color.b > color_range[INDEX_COLOR_MAX].b)
            {
                color_range[INDEX_COLOR_MAX].b = color.b;
            }
        }
    }

    return color_range;
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
