#ifndef __PALETTE_H
#define __PALETTE_H

#include "color.h" // Color structure
#include <climits> // INT_MAX
#include <cmath> // std::abs()
#include <cstdlib> // std::exit
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <string> // std::string
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

class Palette
{
public:
    const std::unordered_map<std::string, std::vector<Color>> PRESET_PALETTES =
    {
        {"1BIT_MONITOR_GLOW", {Color(0x222323), Color(0xf0f6f0)}},
        {"TITANSTONE", {Color(0x150413), Color(0x635c6d), Color(0xffffd1)}},
        {"ICE_CREAM_GB", {Color(0x7c3f58), Color(0xeb6b6f), Color(0xf9a875), Color(0xfff6d3)}},
        {"TWILIGHT5", {Color(0x292831), Color(0x333f58), Color(0x4a7a96), Color(0xee8695), Color(0xfbbbad)}},
        {"OIL6", {Color(0x272744), Color(0x494d7e), Color(0x8b6d9c), Color(0xc69fa5), Color(0xf2d3ab), Color(0xfbf5ef)}},
        {"MIDNIGHT_ABLAZE", {Color(0x130208), Color(0x1f0510), Color(0x31051e), Color(0x460e2b), Color(0x7c183c), Color(0xd53c6a), Color(0xff8274)}},
        {"SLSO8", {Color(0x0d2b45), Color(0x203c56), Color(0x544e68), Color(0x8d697a), Color(0xd08159), Color(0xffaa5e), Color(0xffd4a3), Color(0xffecd6)}}
    };

    Palette() = delete;
    Palette(std::string name);

    std::size_t size();
    Color get_color_at(std::size_t index);
    void sort();
    Color nearest(Color input, std::string mapping_method, std::vector<Color> input_range, std::vector<Color> output_range, std::vector<double> slope);
    int nearest_index_lighter(Color input);
    int nearest_index_darker(Color input);
    Color pitch_vector();
    std::vector<Color> get_color_range();

    std::string to_string();

private:
    std::string name;
    std::vector<Color> colors;
};


#endif
