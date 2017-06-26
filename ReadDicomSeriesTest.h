#ifndef READDICOMSERIESTEST_H
#define READDICOMSERIESTEST_H



#include <iostream>
#include <vector>


#include "gdcmScanner.h"
#include "itkGDCMSeriesFileNames.h"

class ReadDicomSeriesTest
{
public:
    
    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    typedef std::string SeriesUID;
    typedef gdcm::Directory::FilenamesType FileNameContainer;
    
    
    ReadDicomSeriesTest();
    
    void setDirectory(const std::string& dirName);
    
    void scanFilesForTags(const FileNameContainer& filenames);
    
    void readImageSeriesFromDirectory();

    void importMosaicFile(const std::string& fileName);
    
    const FileNameContainer& getFileNamesForSeriesUID(const SeriesUID& uid) const;
    
private:
    std::string dirName;
    NamesGeneratorType::Pointer namesGenerator;
    
    typedef std::unique_ptr<gdcm::Scanner> ScannerPointer;
    
};

#endif // READDICOMSERIESTEST_H
