#include <iostream>
#include "ReadDicomSeriesTest.h"

int main(int argc, char **argv) {
    std::string dirName = "."; //current directory by default
    if (argc > 1)
    {
        dirName = argv[1];
    }

    ReadDicomSeriesTest dicomTest;
    dicomTest.readImageSeriesFromDirectory(dirName);


    return EXIT_SUCCESS;
}
