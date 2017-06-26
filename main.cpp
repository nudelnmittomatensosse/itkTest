#include <iostream>
#include "ReadDicomSeriesTest.h"

int main(int argc, char **argv) {
    std::string dirName = "."; //current directory by default
    if (argc > 1)
    {
        dirName = argv[1];
    }

    ReadDicomSeriesTest dicomTest;
    dicomTest.setDirectory(dirName);
    //  dicomTest.readImageSeriesFromDirectory();
    //    dicomTest.readImageSeriesFromDirectory();

    std::string uid="1.3.12.2.1107.5.2.32.35436.2017061515265315524345438.0.0.0";
    ReadDicomSeriesTest::FileNameContainer fileNames=dicomTest
            .getFileNamesForSeriesUID(uid);

    gdcm::Directory d;
    d.Load(dirName.c_str(), false ); // recursive !
    const gdcm::Directory::FilenamesType &l1 = d.GetFilenames();
    dicomTest.scanFilesForTags(l1);

    dicomTest.importMosaicFile("/home/florian/projects/ITKtest/raw_dicom_files/012_0014_mreepi-40Hz_20170615-153515.ima");




    return EXIT_SUCCESS;
}
