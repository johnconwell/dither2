#include "dither.h"

Dither::Dither()
{
    image = Image();
    return;
}

std::size_t Dither::load(const char* file_name)
{
    return image.load(file_name);
}

std::size_t Dither::save(const char* file_name)
{
    return image.save(file_name);
}

void Dither::grayscale(std::string name_grayscale_weights)
{
    Grayscale grayscale = Grayscale(name_grayscale_weights);
    const std::size_t height = image.get_height();
    const std::size_t width = image.get_width();

    for(std::size_t y = 0; y < height; y++)
    {
        for(std::size_t x = 0; x < width; x++)
        {
            Color color = image.get_pixel(x, y);
            color.to_grayscale(grayscale.weights);
            image.set_pixel(color, x, y);
        }
    }

    return;
}

void Dither::convolve(std::string name_kernel)
{
    Convolve convolve = Convolve(name_kernel);
    const std::size_t image_height = image.get_height();
    const std::size_t image_width = image.get_width();
    const std::size_t kernel_height = convolve.kernel.size();
    const std::size_t kernel_width = convolve.kernel[0].size();
    const std::size_t kernel_height_half = kernel_height / 2;
    const std::size_t kernel_width_half = kernel_width / 2;
    Color color;
    std::vector<std::vector<int>> image_matrix_r;
    std::vector<std::vector<int>> image_matrix_g;
    std::vector<std::vector<int>> image_matrix_b;

    image_matrix_r = std::vector<std::vector<int>>(image_height + 2 * kernel_height_half, std::vector<int>(image_width + 2 * kernel_width_half, 0));
    image_matrix_g = std::vector<std::vector<int>>(image_height + 2 * kernel_height_half, std::vector<int>(image_width + 2 * kernel_width_half, 0));
    image_matrix_b = std::vector<std::vector<int>>(image_height + 2 * kernel_height_half, std::vector<int>(image_width + 2 * kernel_width_half, 0));

    // pad pixels around the top and bottom of the image
    for(std::size_t x = 0; x < image_width; x++)
    {
        // pad around the top of the image
        for(std::size_t y = -kernel_height_half; y < 0; y++)
        {
            color = image.get_pixel(x, 0);
            image_matrix_r[y][x + kernel_width_half] = color.r;
            image_matrix_g[y][x + kernel_width_half] = color.g;
            image_matrix_b[y][x + kernel_width_half] = color.b;
        }

        // pad around the bottom of the image
        for(std::size_t y = image_height; y < image_height + kernel_height_half; y++)
        {
            color = image.get_pixel(x, image_height - 1);
            image_matrix_r[y][x + kernel_width_half] = color.r;
            image_matrix_g[y][x + kernel_width_half] = color.g;
            image_matrix_b[y][x + kernel_width_half] = color.b;
        }
    }

    // pad pixels around the left and right borders of the image
    for(std::size_t y = 0; y < image_height; y++)
    {
        // pad around the left border
        for(std::size_t x = -kernel_width_half; x < 0; x++)
        {
            color = image.get_pixel(0, y);
            image_matrix_r[y + kernel_height_half][x] = color.r;
            image_matrix_g[y + kernel_height_half][x] = color.g;
            image_matrix_b[y + kernel_height_half][x] = color.b;
        }

        // fill in the center of the image
        for(std::size_t x = 0; x < image_width; x++)
        {
            color = image.get_pixel(x, y);
            image_matrix_r[y + kernel_height_half][x + kernel_width_half] = color.r;
            image_matrix_g[y + kernel_height_half][x + kernel_width_half] = color.g;
            image_matrix_b[y + kernel_height_half][x + kernel_width_half] = color.b;
        }

        // pad around the right border
        for(std::size_t x = image_width; x < image_width + kernel_width_half; x++)
        {
            color = image.get_pixel(image_width - 1, y);
            image_matrix_r[y + kernel_height_half][x] = color.r;
            image_matrix_g[y + kernel_height_half][x] = color.g;
            image_matrix_b[y + kernel_height_half][x] = color.b;
        }
    }

    std::vector<std::vector<int>> image_matrix_r_convolved = _convolve<int, double>(image_matrix_r, convolve.kernel);
    std::vector<std::vector<int>> image_matrix_g_convolved = _convolve<int, double>(image_matrix_g, convolve.kernel);
    std::vector<std::vector<int>> image_matrix_b_convolved = _convolve<int, double>(image_matrix_b, convolve.kernel);

    for(std::size_t y = 0; y < image_height; y++)
    {
        for(std::size_t x = 0; x < image_width; x++)
        {
            image_matrix_r_convolved[y][x] = std::clamp(image_matrix_r_convolved[y][x], 0, static_cast<int>(Color::CHANNEL_MAX));
            image_matrix_g_convolved[y][x] = std::clamp(image_matrix_g_convolved[y][x], 0, static_cast<int>(Color::CHANNEL_MAX));
            image_matrix_b_convolved[y][x] = std::clamp(image_matrix_b_convolved[y][x], 0, static_cast<int>(Color::CHANNEL_MAX));
            color = Color(image_matrix_r_convolved[y][x], image_matrix_g_convolved[y][x], image_matrix_b_convolved[y][x], Color::CHANNEL_MAX);
            image.set_pixel(color, x, y);
        }
    }

    return;
}

void Dither::reduce(std::string name_mapping_method, std::string name_palette, bool gamma_correction)
{
    Palette palette = Palette(name_palette);
    const std::size_t image_height = image.get_height();
    const std::size_t image_width = image.get_width();

    for(std::size_t y = 0; y < image_height; y++)
    {
        for(std::size_t x = 0; x < image_width; x++)
        {
            Color color = image.get_pixel(x, y);
            Color palette_nearest;

            if(name_mapping_method == "UNIFORM_HISTOGRAM")
            {
                // TODO
            }
            else
            {
                palette_nearest = palette.nearest(color, name_mapping_method);
            }

            image.set_pixel(palette_nearest, x, y);
        }
    }

    return;
}

void Dither::error_diffusion(std::string name_algorithm, std::string name_mapping_method, std::string name_palette, bool gamma_correction)
{
    ErrorDiffusion error_diffusion = ErrorDiffusion(name_algorithm);
    Palette palette = Palette(name_palette);
    const std::size_t image_height = image.get_height();
    const std::size_t image_width = image.get_width();
    std::vector<std::vector<std::vector<int>>> error_matrix(image_height, std::vector<std::vector<int>>(image_width, std::vector<int>(Color::NUM_BYTES - 1, 0)));

    for(std::size_t y = 0; y < image_height; y++)
    {
        for(std::size_t x = 0; x < image_width; x++)
        {
            // set current pixel to nearest palette color (accounting for accumulated error)
            Color color = image.get_pixel(x, y);

            if(gamma_correction)
            {
                color.to_linear(image.get_gamma());
            }
            
            color.r += error_matrix[y][x][Color::INDEX_R];
            color.g += error_matrix[y][x][Color::INDEX_G];
            color.b += error_matrix[y][x][Color::INDEX_B];

            Color palette_nearest;

            if(name_mapping_method == "UNIFORM_HISTOGRAM")
            {
                // TODO
            }
            else
            {
                palette_nearest = palette.nearest(color, name_mapping_method);
            }

            std::vector<int> current_pixel_error = {color.r - palette_nearest.r, color.g - palette_nearest.g, color.b - palette_nearest.b};

            for(std::size_t index_error = 0; index_error < error_diffusion.coordinates.size(); index_error++)
            {
                const std::size_t new_x = x + error_diffusion.coordinates[index_error].first;
                const std::size_t new_y = y + error_diffusion.coordinates[index_error].second;

                if(new_x < 0 || new_x >= image_width || new_y < 0 || new_y >= image_height)
                {
                    continue;
                }

                error_matrix[new_y][new_x][Color::INDEX_R] += static_cast<int>(current_pixel_error[Color::INDEX_R] * error_diffusion.scalars[error_diffusion.coordinates[index_error]]);
                error_matrix[new_y][new_x][Color::INDEX_G] += static_cast<int>(current_pixel_error[Color::INDEX_G] * error_diffusion.scalars[error_diffusion.coordinates[index_error]]);
                error_matrix[new_y][new_x][Color::INDEX_B] += static_cast<int>(current_pixel_error[Color::INDEX_B] * error_diffusion.scalars[error_diffusion.coordinates[index_error]]);
            }

            image.set_pixel(palette_nearest, x, y);
        }
    }

    return;
}

void Dither::ordered(std::string name_threshold_matrix, std::string name_mapping_method, std::string name_palette, bool gamma_correction)
{
    Ordered ordered = Ordered(name_threshold_matrix);
    Palette palette = Palette(name_palette);
    const std::size_t image_height = image.get_height();
    const std::size_t image_width = image.get_width();
    const std::size_t threshold_matrix_height = ordered.threshold_matrix.size();
    const std::size_t threshold_matrix_width = ordered.threshold_matrix[0].size();

    palette.sort();

    Color palette_pitch_vector = palette.pitch_vector();
    const int color_channel_max_half = Color::CHANNEL_MAX / 2;
    Color color;
    int threshold_value;
    int threshold_value_scaled;
    Color threshold_color_offset;
    Color palette_nearest;

    for(std::size_t y = 0; y < image_height; y++)
    {
        for(std::size_t x = 0; x < image_width; x++)
        {
            color = image.get_pixel(x, y);

            if(gamma_correction)
            {
                color.to_linear(image.get_gamma());
            }
            
            threshold_value = ordered.threshold_matrix[y % threshold_matrix_height][x % threshold_matrix_width];
            // normalize the threshold value by subtracting half the color channel range (normalized values should range from -127 to 128)
            threshold_value_scaled = threshold_value - color_channel_max_half;
            // create a color offset at each position that is the average spread in the color space (palette_pitch_vector / color_channel_max) multiplied by the normalized threshold
            threshold_color_offset = Color(
                threshold_value_scaled * palette_pitch_vector.r / Color::CHANNEL_MAX,
                threshold_value_scaled * palette_pitch_vector.g / Color::CHANNEL_MAX,
                threshold_value_scaled * palette_pitch_vector.b / Color::CHANNEL_MAX,
                Color::CHANNEL_MAX);
            
            if(name_mapping_method == "UNIFORM_HISTOGRAM")
            {
                // TODO
            }
            else
            {
                palette_nearest = palette.nearest(color.add(threshold_color_offset), name_mapping_method);
            }
            
            image.set_pixel(palette_nearest, x, y);
        }
    }

    return;
}
