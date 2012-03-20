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
#ifndef __otbLabelImageToOGRDataSourceFilter_txx
#define __otbLabelImageToOGRDataSourceFilter_txx

#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbGdalDataTypeBridge.h"


//gdal libraries
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal_alg.h"
#include "ogrsf_frmts.h"

#include <typeinfo>
namespace otb
{

class OGRDataSourceWrapper : public itk::Object
{

public:
  typedef OGRDataSourceWrapper      Self;
  typedef itk::Object        Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRDataSourceWrapper, itk::Object);

  void SetDataSource (OGRDataSource* datasource)
  {
      m_DataSource = datasource;
  }
  /** Easy access to the internal OGRDataSource object.
   *  Don't close it, it will be automatic */
  OGRDataSource* GetDataSource() const
    {
    return m_DataSource;
    }

protected :
  OGRDataSourceWrapper()
   : m_DataSource(NULL)
  {
  }

  ~OGRDataSourceWrapper()
  {
    if (m_DataSource)
      {
      OGRDataSource::DestroyDataSource(m_DataSource);
      }
  }

private:
  OGRDataSource* m_DataSource;
}; // end of OGRDataSourceWrapper


template <class TInputImage>
LabelImageToOGRDataSourceFilter<TInputImage>
::LabelImageToOGRDataSourceFilter() : m_FieldName("DN")
{
   this->SetNumberOfRequiredInputs(1);
   this->SetNumberOfRequiredOutputs(1);
   
   GDALAllRegister();
   OGRRegisterAll();
   
   this->ProcessObject::SetNthOutput(0, this->MakeOutput(0) );
}


template <class TInputImage>
typename LabelImageToOGRDataSourceFilter<TInputImage>::DataObjectPointer
LabelImageToOGRDataSourceFilter<TInputImage>
::MakeOutput(unsigned int itkNotUsed(idx))
{
  return static_cast< DataObjectPointer >(OGRDataSourceObjectType::New().GetPointer());
}

template <class TInputImage>
const typename LabelImageToOGRDataSourceFilter<TInputImage>::OGRDataSourceObjectType *
LabelImageToOGRDataSourceFilter<TInputImage>
::GetOutput()
{
  return static_cast< const OGRDataSourceObjectType * >(
              this->ProcessObject::GetOutput(0));
}

template <class TInputImage>
void
LabelImageToOGRDataSourceFilter<TInputImage>
::SetInput(const InputImageType *input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage>
const typename LabelImageToOGRDataSourceFilter<TInputImage>
::InputImageType *
LabelImageToOGRDataSourceFilter<TInputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(0));
}

template <class TInputImage>
void
LabelImageToOGRDataSourceFilter<TInputImage>
::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input  =
    const_cast<InputImageType *> (this->GetInput());

  if ( !input )
    {
    return;
    }

  // The input is necessarily the largest possible region.
  // For a streamed implementation, use the StreamingLineSegmentDetector filter
  input->SetRequestedRegionToLargestPossibleRegion();
}


template <class TInputImage>
void
LabelImageToOGRDataSourceFilter<TInputImage>
::GenerateData(void)
{
   if (this->GetInput()->GetRequestedRegion() != this->GetInput()->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<< "Not streamed filter. ERROR : requested region is not the largest possible region.");
    }
    
    typename InputImageType::Pointer inImage = const_cast<InputImageType *>(this->GetInput());

    SizeType size = this->GetInput()->GetLargestPossibleRegion().GetSize();
    
    unsigned int nbBands = this->GetInput()->GetNumberOfComponentsPerPixel();
    unsigned int bytePerPixel = sizeof(InputPixelType);

    /** Convert Input image into a OGRLayer using GDALPolygonize */
    // buffer casted in unsigned long cause under Win32 the adress
    // don't begin with 0x, the adress in not interpreted as
    // hexadecimal but alpha numeric value, then the conversion to
    // integer make us pointing to an non allowed memory block => Crash.
    std::ostringstream stream;
    stream << "MEM:::"
           <<  "DATAPOINTER=" << (unsigned long)(this->GetInput()->GetBufferPointer()) << ","
           <<  "PIXELS=" << size[0] << ","
           <<  "LINES=" << size[1] << ","
           <<  "BANDS=" << nbBands << ","
           <<  "DATATYPE=" << GDALGetDataTypeName(GdalDataTypeBridge::GetGDALDataType<InputPixelType>()) << ","
           <<  "PIXELOFFSET=" << bytePerPixel * nbBands << ","
           <<  "LINEOFFSET=" << bytePerPixel * nbBands * size[0] << ","
           <<  "BANDOFFSET=" << bytePerPixel;
    
    GDALDataset * dataset = static_cast<GDALDataset *> (GDALOpen(stream.str().c_str(), GA_ReadOnly));
    
    //Set input Projection ref and Geo transform to the dataset.
    dataset->SetProjection(this->GetInput()->GetProjectionRef().c_str());
    
    unsigned int projSize = this->GetInput()->GetGeoTransform().size();
    double geoTransform[6];
    
    //Set the geo transform of the input image (if any)
    // Reporting origin and spacing of the buffered region
    // the spacing is unchanged, the origin is relative to the buffered region
    IndexType  bufferIndexOrigin = this->GetInput()->GetBufferedRegion().GetIndex();
    OriginType  bufferOrigin;
    this->GetInput()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
    geoTransform[0] = bufferOrigin[0];
    geoTransform[3] = bufferOrigin[1];
    geoTransform[1] = this->GetInput()->GetSpacing()[0];
    geoTransform[5] = this->GetInput()->GetSpacing()[1];
    // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
    if (projSize == 0)
    {
      geoTransform[2] = 0.;
      geoTransform[4] = 0.;
    }
    else
    {
      geoTransform[2] = this->GetInput()->GetGeoTransform()[2];
      geoTransform[4] = this->GetInput()->GetGeoTransform()[4];
    }
    dataset->SetGeoTransform(geoTransform);
    
    //Create the output layer for GDALPolygonize().
    const char * driverName = "Memory";
    OGRSFDriver * ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName);
    OGRDataSource * dataSource = ogrDriver->CreateDataSource("Shape",NULL);
    
    OGRLayer * outputLayer = dataSource->CreateLayer("layer",NULL,wkbMultiPolygon,NULL);
    
    OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
    outputLayer->CreateField(&field, true);

    //Call GDALPolygonize()
    GDALPolygonize(dataset->GetRasterBand(1), NULL, outputLayer, 0, NULL, NULL, NULL);
    
    
    OGRDataSourceObjectType * decoratedOutput =
            static_cast< OGRDataSourceObjectType * >(
              this->ProcessObject::GetOutput(0));
              
    typename OGRDataSourceWrapper::Pointer dataSourceWrapper = OGRDataSourceWrapper::New();
    dataSourceWrapper->SetDataSource(dataSource);
    decoratedOutput->Set(dataSourceWrapper);
    
    //Clear memory
    GDALClose(dataset);

}


} // end namespace otb

#endif