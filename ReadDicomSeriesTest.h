#ifndef READDICOMSERIESTEST_H
#define READDICOMSERIESTEST_H



#include <iostream>
#include <vector>


#include "gdcmScanner.h"

class ReadDicomSeriesTest
{
public:
    
    typedef gdcm::Directory::FilenamesType FileNameContainer;
    
    void scanFilesForTags(const FileNameContainer& filenames);
    
    void readImageSeriesFromDirectory(const std::string& dirName);
    
private:
    typedef std::unique_ptr<gdcm::Scanner> ScannerPointer;
    
};

#endif // READDICOMSERIESTEST_H
