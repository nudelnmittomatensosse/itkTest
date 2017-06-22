#include "ReadDicomSeriesTest.h"


#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"

#include "itkNiftiImageIO.h"
#include "itkImageFileWriter.h"

#include "gdcmScanner.h"

void ReadDicomSeriesTest::scanFilesForTags(const FileNameContainer& filenames)
{
    const gdcm::Tag tag_array[] = {
        gdcm::Tag(0x8,0x50),
        gdcm::Tag(0x8,0x51),
        gdcm::Tag(0x8,0x60),
    };
    
    std::unique_ptr<gdcm::Scanner> fileScanner(new gdcm::Scanner());
    for (auto tag:tag_array)
    {
        fileScanner->AddTag(tag);                
    }
    
    if( fileScanner->Scan( filenames ) )
    {
        
        
    }
    
    
}




void ReadDicomSeriesTest::readImageSeriesFromDirectory(const std::string& dirName)
{
    typedef short    PixelType;
    const unsigned int      Dimension = 3;
    typedef itk::Image< PixelType, Dimension >         ImageType;
    typedef itk::ImageSeriesReader< ImageType >        ReaderType;


    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetDirectory(dirName);

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

        ReaderType::Pointer reader = ReaderType::New();
        typedef itk::GDCMImageIO       ImageIOType;
        ImageIOType::Pointer dicomIO = ImageIOType::New();
        reader->SetImageIO(dicomIO);
        seriesItr = seriesUID.begin();
        reader->SetFileNames(nameGenerator->GetFileNames(seriesItr->c_str()));

        itk::NiftiImageIO::Pointer nifti_io = itk::NiftiImageIO::New();

        typedef itk::Image<short, 3> DWI;
        itk::ImageFileWriter<DWI>::Pointer dwi_writer = itk::ImageFileWriter<DWI>::New();
        dwi_writer->SetFileName("test.nii.gz");
        dwi_writer->SetInput(reader->GetOutput());
        dwi_writer->SetImageIO(nifti_io);
        dwi_writer->Update();


    } catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
    }

}
