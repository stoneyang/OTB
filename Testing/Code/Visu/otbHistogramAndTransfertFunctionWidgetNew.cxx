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
#include "itkScalarImageToHistogramGenerator.h"
#include "otbHistogramAndTransfertFunctionWidget.h"
#include "otbImage.h"

int otbHistogramAndTransfertFunctionWidgetNew(int argc, char * argv[])
{
  typedef unsigned char PixelType;
  const unsigned int Dimension =2;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef itk::Statistics::ScalarImageToHistogramGenerator<ImageType> GeneratorType;
  typedef GeneratorType::HistogramType HistogramType;

  typedef otb::HistogramAndTransfertFunctionWidget<HistogramType> WidgetType;

  WidgetType::Pointer widget = WidgetType::New();
  
  return EXIT_SUCCESS;
}
