#ifndef __DITHER_H
#define __DITHER_H

#include "convolve.h"
#include "error_diffusion.h"
#include "grayscale.h"
#include "image.h"
#include "ordered.h"
#include "palette.h"
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

class Dither
{
public:
    Dither();

    void grayscale(std::string name_grayscale_weights);
    void convolve(std::string name_kernel);
    void reduce(std::string name_mapping_method, std::string name_palette, bool gamma_correction);
    void error_diffusion(std::string name_algorithm, std::string name_mapping_method, std::string name_palette, bool gamma_correction);
    void ordered(std::string name_threshold_matrix, std::string name_mapping_method, std::string name_palette, bool gamma_correction);

private:
    Image image;
};










#endif
