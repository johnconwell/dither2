#ifndef __GRAYSCALE_H
#define __GRAYSCALE_H

#include <cstdlib> // std::exit
#include <iostream> // std::cerr, std::endl
#include <string> // std::string
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

class Grayscale
{
public:
    const std::unordered_map<std::string, std::vector<double>> METHODS =
    {
        {"STANDARD", {0.3334, 0.3333, 0.3333}},
        {"BT709", {0.2126, 0.7152, 0.0722}},
        {"CHANNEL_R", {1.0, 0.0, 0.0}},
        {"CHANNEL_G", {0.0, 1.0, 0.0}},
        {"CHANNEL_B", {0.0, 0.0, 1.0}},
        {"CHANNEL_RG", {0.5, 0.5, 0.0}},
        {"CHANNEL_RB", {0.5, 0.0, 0.5}},
        {"CHANNEL_GB", {0.0, 0.5, 0.5}}
    };

    Grayscale() = delete;

    Grayscale(std::string name)
    {
        if(!METHODS.contains(name))
        {
            std::cerr << "Error: innvalid grayscale method - " << name << std::endl;
            std::exit(EXIT_FAILURE);
        }

        this->name = name;
        this->weights = METHODS.at(name);
    }

    std::string name;
    std::vector<double> weights;
};

#endif
