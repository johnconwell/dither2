#ifndef __CONVOLVE_H
#define __CONVOLVE_H

#include <iostream> // std::cerr, std::endl
#include <string> // std::string
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

class Convolve
{
public:
    const std::unordered_map<std::string, std::vector<std::vector<double>>> KERNELS =
    {
        {"RIDGE_4", std::vector<std::vector<double>>({
            {+0.0, -1.0, +0.0},
            {-1.0, +4.0, -1.0},
            {+0.0, -1.0, +0.0}
        })},
        {"RIDGE_8", std::vector<std::vector<double>>({
            {-1.0, -1.0, -1.0},
            {-1.0, +8.0, -1.0},
            {-1.0, -1.0, -1.0}
        })},
        {"SHARPEN_4", std::vector<std::vector<double>>({
            {+0.0, -1.0, +0.0},
            {-1.0, +5.0, -1.0},
            {+0.0, -1.0, +0.0}
        })},
        {"SHARPEN_8", std::vector<std::vector<double>>({
            {-1.0, -1.0, -1.0},
            {-1.0, +9.0, -1.0},
            {-1.0, -1.0, -1.0}
        })},
        {"BOX_BLUR", std::vector<std::vector<double>>({
            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
        })},
        {"GAUSSIAN_BLUR", std::vector<std::vector<double>>({
            {0.00296902, 0.0133062, 0.0219382, 0.0133062, 0.00296902},
            {0.0133062, 0.0596343, 0.0983203, 0.0596343, 0.0133062},
            {0.0219382, 0.0983203, 0.162103, 0.0983203, 0.0219382},
            {0.0133062, 0.0596343, 0.0983203, 0.0596343, 0.0133062},
            {0.00296902, 0.0133062, 0.0219382, 0.0133062, 0.00296902}
        })},
        {"UNSHARP_MASK", std::vector<std::vector<double>>({
            {-0.00296902, -0.0133062, -0.0219382, -0.0133062, -0.00296902},
            {-0.0133062, -0.0596343, -0.0983203, -0.0596343, -0.0133062},
            {-0.0219382, -0.0983203, +1.837897, -0.0983203, -0.0219382},
            {-0.0133062, -0.0596343, -0.0983203, -0.0596343, -0.0133062},
            {-0.00296902, -0.0133062, -0.0219382, -0.0133062, -0.00296902}
        })}
    };

    Convolve() = delete;

    Convolve(std::string name)
    {
        if(!KERNELS.contains(name))
        {
            std::cerr << "Error: invalid convolution kernel - " << name << std::endl;
            std::exit(EXIT_FAILURE);
        }

        this->name = name;
        this->kernel = KERNELS.at(name);
    }

    std::string name;
    std::vector<std::vector<double>> kernel;
};


#endif
