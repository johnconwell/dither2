#include "cli.h"
#include "dither.h"
#include "image.h"
#include "palette.h"
#include <cstdlib> // std::exit
#include <iostream>



int main(int argc, const char* argv[])
{
    CLI cli = CLI();

    if(!cli.parse(argc, argv))
    {
        return EXIT_FAILURE;
    }

    Image image;
    image.load(cli.file_path_input.c_str());
    image.save(cli.file_path_output.c_str());

    return EXIT_SUCCESS;
}
