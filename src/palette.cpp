#include "palette.h"

Palette::Palette(std::string name)
{
    if(PRESET_PALETTES.contains(name))
    {
        this->name = name;
        this->colors = PRESET_PALETTES.at(name);
    }
    else
    {
        std::ifstream file;
        file.open(name);
        
        if(!file.is_open())
        {
            std::cerr << "Error: innvalid palette argument - " << name << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string line;
        std::vector<std::string> tokens;
        std::string token;

        std::getline(file, line);
        std::stringstream ss(line);
        
        while(std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        this->name = tokens[0];

        for(std::size_t index_tokens = 2; index_tokens < tokens.size(); index_tokens++)
        {
            colors.push_back(Color(std::stoi(tokens[index_tokens], nullptr, 16)));
        }
    }

    return;
}

void Palette::sort()
{
    std::size_t colors_size = colors.size();

    for(std::size_t index_primary = 0; index_primary < colors_size - 1; index_primary++)
    {
        for(std::size_t index_secondary = index_primary + 1; index_secondary < colors_size; index_secondary++)
        {
            if(colors[index_secondary].get_lightness() < colors[index_primary].get_lightness())
            {
                Color temp = colors[index_primary];
                colors[index_primary] = colors[index_secondary];
                colors[index_secondary] = temp;
            }
        }
    }

    return;
}

Color Palette::nearest(Color color, std::string mapping_method)
{
    int index_nearest = -1;
    int distance_squared_nearest = INT_MAX;
    std::size_t colors_size = colors.size();

    if(mapping_method == "EUCLIDEAN_DISTANCE")
    {
        for(std::size_t index_palette = 0; index_palette < colors_size; index_palette++)
        {
            int distance_squared = color.distance_euclidean_squared(colors[index_palette]);
            if(distance_squared < distance_squared_nearest)
            {
                distance_squared_nearest = distance_squared;
                index_nearest = index_palette;
            }
        }
    }
    else if(mapping_method == "MANHATTAN_DISTANCE")
    {
        for(std::size_t index_palette = 0; index_palette < colors_size; index_palette++)
        {
            int distance_squared = color.distance_manhattan(colors[index_palette]);
            if(distance_squared < distance_squared_nearest)
            {
                distance_squared_nearest = distance_squared;
                index_nearest = index_palette;
            }
        }
    }

    return colors[index_nearest];
}

Color Palette::pitch_vector()
{
    std::size_t colors_size = colors.size();
    Color distance_vector = Color(0, 0, 0, Color::CHANNEL_MAX);

    for(size_t index_colors = 0; index_colors < colors_size - 1; index_colors++)
    {
        distance_vector.r += std::abs(colors[index_colors].r - colors[index_colors + 1].r);
        distance_vector.g += std::abs(colors[index_colors].g - colors[index_colors + 1].g);
        distance_vector.b += std::abs(colors[index_colors].b - colors[index_colors + 1].b);
    }

    distance_vector.r /= static_cast<double>(colors_size - 1);
    distance_vector.g /= static_cast<double>(colors_size - 1);
    distance_vector.b /= static_cast<double>(colors_size - 1);

    return distance_vector;
}

std::string Palette::to_string()
{
    std::string output = name + "\n";
    size_t colors_size = colors.size();

    for(size_t index_palette = 0; index_palette < colors_size; index_palette++)
    {
        output += colors[index_palette].to_string_hex() + "\n";
    }

    return output;
}
