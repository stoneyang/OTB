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


#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbLabelImageToOGRDataSourceFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToOGRDataSourceFilter<InputLabelImageType>  FilterType;
  
  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}


int otbLabelImageToOGRDataSourceFilter(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputLabelImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
    }
  const char * infname = argv[1];
  const char * outfname = argv[2];
  
  
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToOGRDataSourceFilter<InputLabelImageType>   FilterType;
  typedef otb::ImageFileReader<InputLabelImageType>                LabelImageReaderType;


  FilterType::Pointer filter = FilterType::New();
  LabelImageReaderType::Pointer reader = LabelImageReaderType::New();
  
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());
  filter->Update();
  
  std::cout<<"layer name : "<<filter->GetOutput()->Get()->GetDataSource()->GetLayer(0)->GetName()<<std::endl;
  std::cout<<"layer nb features : "<<filter->GetOutput()->Get()->GetDataSource()->GetLayer(0)->GetFeatureCount()<<std::endl;

  return EXIT_SUCCESS;
}