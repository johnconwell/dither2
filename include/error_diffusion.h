#ifndef __ERROR_DIFFUSION_H
#define __ERROR_DIFFUSION_H

#include <iostream> // std::cerr, std::endl
#include <string> // std::string
#include <unordered_map> // std::unordered_map
#include <utility> // std::pair
#include <vector> // std::vector

// required for dictionary of std::pair<int, int>
struct PairHash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class ErrorDiffusion
{
public:
    static inline constexpr std::pair<int, int> R    {1,  0};
    static inline constexpr std::pair<int, int> RR   {2,  0};
    static inline constexpr std::pair<int, int> LLD  {-2, 1};
    static inline constexpr std::pair<int, int> LD   {-1, 1};
    static inline constexpr std::pair<int, int> D    {0,  1};
    static inline constexpr std::pair<int, int> RD   {1,  1};
    static inline constexpr std::pair<int, int> RRD  {2,  1};
    static inline constexpr std::pair<int, int> LLDD {-2, 2};
    static inline constexpr std::pair<int, int> LDD  {-1, 2};
    static inline constexpr std::pair<int, int> DD   {0,  2};
    static inline constexpr std::pair<int, int> RDD  {1,  2};
    static inline constexpr std::pair<int, int> RRDD {2,  2};

    const std::unordered_map<std::string, std::vector<std::pair<int, int>>> COORDINATES =
    {
        {"LINEAR", {R}},
        {"FLOYD_STEINBERG", {R, LD, D, RD}},
        {"JARVICE_JUDICE_NINKE", {R, RR, LLD, LD, D, RD, RRD, LLDD, LDD, DD, RDD, RRDD}},
        {"STUCKI", {R, RR, LLD, LD, D, RD, RRD, LLDD, LDD, DD, RDD, RRDD}},
        {"ATKINSON", {R, RR, LD, D, RD, DD}},
        {"BURKES", {R, RR, LLD, LD, D, RD, RRD}},
        {"SIERRA", {R, RR, LLD, LD, D, RD, RRD, LDD, DD, RDD}},
        {"SIERRA_TWO_ROW", {R, RR, LLD, LD, D, RD, RRD}},
        {"SIERRA_LITE", {R, LD, D}}
    };

    const std::unordered_map<std::string, std::unordered_map<std::pair<int, int>, const double, PairHash>> SCALARS =
    {
        {"LINEAR", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 1.0}
        })},
        {"FLOYD_STEINBERG", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 7.0 / 16.0},
            {LD, 3.0 / 16.0}, {D, 5.0 / 16.0}, {RD, 1.0 / 16.0}
        })},
        {"JARVICE_JUDICE_NINKE", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 7.0 / 48.0}, {RR, 5.0 / 48.0},
            {LLD, 3.0 / 48.0}, {LD, 5.0 / 48.0}, {D, 7.0 / 48.0}, {RD, 5.0 / 48.0}, {RRD, 3.0 / 48.0},
            {LLDD, 1.0 / 48.0}, {LDD, 3.0 / 48.0}, {DD, 5.0 / 48.0}, {RDD, 3.0 / 48.0}, {RRDD, 1.0 / 48.0}
        })},
        {"STUCKI", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 8.0 / 42.0}, {RR, 4.0 / 42.0},
            {LLD, 2.0 / 42.0}, {LD, 4.0 / 42.0}, {D, 8.0 / 42.0}, {RD, 4.0 / 42.0}, {RRD, 2.0 / 42.0},
            {LLDD, 1.0 / 42.0}, {LDD, 2.0 / 42.0}, {DD, 4.0 / 42.0}, {RDD, 2.0 / 42.0}, {RRDD, 1.0 / 42.0}
        })},
        {"ATKINSON", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 1.0 / 8.0}, {RR, 1.0 / 8.0},
            {LD, 1.0 / 8.0}, {D, 1.0 / 8.0}, {RD, 1.0 / 8.0},
            {DD, 1.0 / 8.0}
        })},
        {"BURKES", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 8.0 / 32.0}, {RR, 4.0 / 32.0},
            {LLD, 2.0 / 32.0}, {LD, 4.0 / 32.0}, {D, 8.0 / 32.0}, {RD, 4.0 / 32.0}, {RRD, 2.0 / 32.0}
        })},
        {"SIERRA", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 5.0 / 32.0}, {RR, 3.0 / 32.0},
            {LLD, 2.0 / 32.0}, {LD, 4.0 / 32.0}, {D, 5.0 / 32.0}, {RD, 4.0 / 32.0}, {RRD, 2.0 / 32.0},
            {LDD, 2.0 / 32.0}, {DD, 3.0 / 32.0}, {RDD, 2.0 / 32.0}
        })},
        {"SIERRA_TWO_ROW", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 4.0 / 16.0}, {RR, 3.0 / 16.0},
            {LLD, 1.0 / 16.0}, {LD, 2.0 / 16.0}, {D, 3.0 / 16.0}, {RD, 2.0 / 16.0}, {RRD, 1.0 / 16.0}
        })},
        {"SIERRA_LITE", std::unordered_map<std::pair<int, int>, const double, PairHash>({
            {R, 2.0 / 4.0},
            {LD, 1.0 / 4.0}, {D, 1.0 / 4.0}
        })}
    };

    ErrorDiffusion() = delete;

    ErrorDiffusion::ErrorDiffusion(std::string name)
    {
        if(!COORDINATES.contains(name))
        {
            std::cerr << "Error: invalid error diffusion algorithm - " << name << std::endl;
            std::exit(EXIT_FAILURE);
        }

        this->name = name;
        this->coordinates = COORDINATES.at(name);
        this->scalars = SCALARS.at(name);
    }

    std::string name;
    std::vector<std::pair<int, int>> coordinates;
    std::unordered_map<std::pair<int, int>, const double, PairHash> scalars;
};

#endif
