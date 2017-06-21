#include "ReadDicomSeriesTest.h"


#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"

void ReadDicomSeriesTest::readImageSeriesFromDirectory(const std::string& dirName)
{
    typedef signed short    PixelType;
    const unsigned int      Dimension = 3;
    typedef itk::Image< PixelType, Dimension >         ImageType;
    typedef itk::ImageSeriesReader< ImageType >        ReaderType;


    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetDirectory(dirName);
    nameGenerator->GetFileNames();

    try
    {
        typedef std::vector< std::string >    SeriesIdContainer;
        const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
        SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
        SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();

        if (seriesItr != seriesEnd)
        {
            std::cout << "The directory: ";
            std::cout << dirName << std::endl;
            std::cout << "Contains the following DICOM Series: ";
            std::cout << std::endl;
        }
        else
        {
            std::cout << "No DICOMs in: " << dirName << std::endl;
        }

        while (seriesItr != seriesEnd)
        {
            std::cout << seriesItr->c_str() << std::endl;
            ++seriesItr;
        }
        
        typedef std::vector< std::string >   FileNamesContainer;
        FileNamesContainer fileNames = nameGenerator->GetOutputFileNames();

        ReaderType::Pointer reader = ReaderType::New();
        typedef itk::GDCMImageIO       ImageIOType;
        ImageIOType::Pointer dicomIO = ImageIOType::New();
        reader->SetImageIO(dicomIO);
        reader->SetFileNames(fileNames);
        
        reader->Update();


    } catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
    }
    
}
