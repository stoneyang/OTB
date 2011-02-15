/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "otbStreamingStatisticsVectorImageFilter2.h"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include <fstream>
#include "otbStreamingTraits.h"

const unsigned int Dimension = 2;
typedef float PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::StreamingStatisticsVectorImageFilter2<ImageType> StreamingStatisticsVectorImageFilterType;

int otbStreamingStatisticsVectorImageFilter2NewTest(int argc, char * argv[])
{
  StreamingStatisticsVectorImageFilterType::Pointer filter = StreamingStatisticsVectorImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbStreamingStatisticsVectorImageFilter2Test(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  StreamingStatisticsVectorImageFilterType::Pointer filter = StreamingStatisticsVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  //filter->SetStreamingMode(otb::SET_NUMBER_OF_STREAM_DIVISIONS);
  //filter->GetStreamer()->SetNumberOfStreamDivisions(200);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file.precision(5);
  file << "Mean: " << filter->GetMean() << std::endl;
  file << "Covariance: " << filter->GetCovariance() << std::endl;
  file << "Correlation: " << filter->GetCorrelation() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
