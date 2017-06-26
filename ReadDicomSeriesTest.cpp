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
#include <gdcmSplitMosaicFilter.h>
#include "gdcmReader.h"
#include "gdcmImageReader.h"
#include "gdcmImageWriter.h"



ReadDicomSeriesTest::ReadDicomSeriesTest()
    :dirName(".")
{
    this->namesGenerator=NamesGeneratorType::New();
    
}

void ReadDicomSeriesTest::setDirectory(const std::string& dirName)
{
    this->dirName=dirName;
    this->namesGenerator->SetDirectory(dirName);
    
}


void ReadDicomSeriesTest::scanFilesForTags(const FileNameContainer& filenames)
{
    const gdcm::Tag tag_array[] = {
        gdcm::Tag(0x8,0x8),
        gdcm::Tag(0x20,0x11),
        gdcm::Tag(0x10,0x10),
    };

    std::unique_ptr<gdcm::Scanner> fileScanner(new gdcm::Scanner());
    for (auto tag:tag_array)
    {
        fileScanner->AddTag(tag);
    }

    if( fileScanner->Scan( filenames ) )
    {
        FileNameContainer::const_iterator currFileIter=filenames.begin();
        while (currFileIter != filenames.end())
        {
            std::cout << currFileIter->c_str() << std::endl;

            gdcm::Scanner::TagToValue const &ttv = fileScanner->GetMapping(currFileIter->c_str());
            for (auto tag:tag_array)
            {
                gdcm::Scanner::TagToValue::const_iterator it = ttv.find( tag );
                if( it != ttv.end() )
                {
                    const char *value = it->second;
                    if( *value )
                    {
                        std::cout << tag << "  has the value: " << value << std::endl;
                    }
                    else
                    {
                        std::cout << tag << " has no value (empty)" << std::endl;
                    }
                }
                else
                {
                    std::cout << tag << " could not be found in this file" << std::endl;
                }


            }
            currFileIter++;
        }

    }

}


void ReadDicomSeriesTest::readImageSeriesFromDirectory()
{
    typedef short    PixelType;
    const unsigned int      Dimension = 3;
    typedef itk::Image< PixelType, Dimension >         ImageType;
    typedef itk::ImageSeriesReader< ImageType >        ReaderType;

    try
    {
        typedef std::vector< SeriesUID>    SeriesIdContainer;
        const SeriesIdContainer & seriesUID = this->namesGenerator->GetSeriesUIDs();
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
        reader->SetFileNames(this->getFileNamesForSeriesUID(*seriesItr));

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

const ReadDicomSeriesTest::FileNameContainer&
    ReadDicomSeriesTest::getFileNamesForSeriesUID(const SeriesUID& uid) const
{
    return this->namesGenerator->GetFileNames(uid.c_str());
    
}

void ReadDicomSeriesTest::importMosaicFile(const std::string &fileName)
{
    gdcm::Reader readerHeader;
    readerHeader.SetFileName( fileName.c_str() );
    gdcm::ImageReader readerImage;
    readerImage.SetFileName(fileName.c_str() );
    if( readerHeader.Read() &&  readerImage.Read())
    {
       const gdcm::File &file1 = readerHeader.GetFile();
       gdcm::SplitMosaicFilter filter;
       filter.SetFile(file1);
       filter.SetImage(readerImage.GetImage());
       if (filter.Split())
       {
           std::cout << "MOSAIC is split." << std::endl;
           gdcm::ImageWriter writer;
           writer.SetFile(filter.GetFile());
           writer.SetImage(filter.GetImage());
           writer.SetFileName( "splitMosaic.dcm" );
           if( writer.Write() )
           {
               std::cout << "Split Mosaic was written." << std::endl;
           }
       }
    }



}

