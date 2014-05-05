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
#ifndef __otbScalarImageToAdvancedTexturesFilter_txx
#define __otbScalarImageToAdvancedTexturesFilter_txx

#include "otbScalarImageToAdvancedTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"
#include <algorithm>

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToAdvancedTexturesFilter() : m_Radius(),
  m_Offset(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(255)
{
  // There are 10 outputs corresponding to the 9 textures indices
  this->SetNumberOfRequiredOutputs(10);

  // Create the 10 outputs
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());
  this->SetNthOutput(4, OutputImageType::New());
  this->SetNthOutput(5, OutputImageType::New());
  this->SetNthOutput(6, OutputImageType::New());
  this->SetNthOutput(7, OutputImageType::New());
  this->SetNthOutput(8, OutputImageType::New());
  this->SetNthOutput(9, OutputImageType::New());
}

template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::~ScalarImageToAdvancedTexturesFilter()
{}
template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetMeanOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDissimilarityOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(2));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumAverageOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(3));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(4));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(5));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 7)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(6));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 8)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(7));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC1Output()
{
  if (this->GetNumberOfOutputs() < 9)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(8));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC2Output()
{
  if (this->GetNumberOfOutputs() < 10)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(9));
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  typename OutputRegionType::IndexType outputIndex = outputRequestedRegion.GetIndex();
  typename OutputRegionType::SizeType  outputSize   = outputRequestedRegion.GetSize();
  typename InputRegionType::IndexType  inputIndex;
  typename InputRegionType::SizeType   inputSize;

  // First, apply offset
  for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
    {
    inputIndex[dim] = std::min(outputIndex[dim], outputIndex[dim] + m_Offset[dim]);
    inputSize[dim] =
      std::max(outputIndex[dim] + outputSize[dim], outputIndex[dim] + outputSize[dim] +
               m_Offset[dim]) - inputIndex[dim];
    }

  // Build the input requested region
  InputRegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(inputIndex);
  inputRequestedRegion.SetSize(inputSize);

  // Apply the radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // Try to apply the requested region to the input image
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}


template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  /** Initalize m_Histogram with given min, max and number of bins  **/
  MeasurementVectorType m_LowerBound;
  MeasurementVectorType m_UpperBound;
  m_Histogram = HistogramType::New();
  m_Histogram->SetMeasurementVectorSize( MeasurementVectorSize );
  m_LowerBound.SetSize( MeasurementVectorSize );
  m_UpperBound.SetSize( MeasurementVectorSize );
  m_LowerBound.Fill(m_InputImageMinimum);
  m_UpperBound.Fill(m_InputImageMaximum+1);
  typename HistogramType::SizeType size( MeasurementVectorSize );
  size.Fill(m_NumberOfBinsPerAxis);
  m_Histogram->Initialize(size, m_LowerBound, m_UpperBound);

  /** calulate minimum offset and set it as neigborhood radius **/
  unsigned int minRadius = 0;
  for ( unsigned int i = 0; i < m_Offset.GetOffsetDimension(); i++ )
    {
    unsigned int distance = vcl_abs(m_Offset[i]);
    if ( distance > minRadius )
      {
      minRadius = distance;
      }
    }
  m_NeighborhoodRadius.Fill(minRadius);
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr              =      const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType meanPtr               =      this->GetMeanOutput();
  OutputImagePointerType variancePtr           =      this->GetVarianceOutput();
  OutputImagePointerType dissimilarityPtr      =      this->GetDissimilarityOutput();
  OutputImagePointerType sumAveragePtr         =      this->GetSumAverageOutput();
  OutputImagePointerType sumVariancePtr        =      this->GetSumVarianceOutput();
  OutputImagePointerType sumEntropytPtr        =      this->GetSumEntropyOutput();
  OutputImagePointerType differenceEntropyPtr  =      this->GetDifferenceEntropyOutput();
  OutputImagePointerType differenceVariancePtr =      this->GetDifferenceVarianceOutput();
  OutputImagePointerType ic1Ptr                =      this->GetIC1Output();
  OutputImagePointerType ic2Ptr                =      this->GetIC2Output();

  // Build output iterators
  itk::ImageRegionIteratorWithIndex<OutputImageType> varianceIt(variancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          meanIt(meanPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          dissimilarityIt(dissimilarityPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumAverageIt(sumAveragePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumVarianceIt(sumVariancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumEntropytIt(sumEntropytPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          differenceEntropyIt(differenceEntropyPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          differenceVarianceIt(differenceVariancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          ic1It(ic1Ptr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          ic2It(ic2Ptr, outputRegionForThread);

  // Go to begin
  varianceIt.GoToBegin();
  meanIt.GoToBegin();
  dissimilarityIt.GoToBegin();
  sumAverageIt.GoToBegin();
  sumVarianceIt.GoToBegin();
  sumEntropytIt.GoToBegin();
  differenceEntropyIt.GoToBegin();
  differenceVarianceIt.GoToBegin();
  ic1It.GoToBegin();
  ic2It.GoToBegin();

  const double log2 = vcl_log(2.0);

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!varianceIt.IsAtEnd()
         && !meanIt.IsAtEnd()
         && !dissimilarityIt.IsAtEnd()
         && !sumAverageIt.IsAtEnd()
         && !sumVarianceIt.IsAtEnd()
         && !sumEntropytIt.IsAtEnd()
         && !differenceEntropyIt.IsAtEnd()
         && !differenceVarianceIt.IsAtEnd()
         && !ic1It.IsAtEnd()
         && !ic2It.IsAtEnd())
    {
    // Compute the region on which co-occurence will be estimated
    typename InputRegionType::IndexType inputIndex;
    typename InputRegionType::SizeType inputSize;

    // First, create an window for neighborhood iterator based on m_Radius
    // For example, if xradius and yradius is 2. window size is 5x5 (2 * radius + 1).
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      inputIndex[dim] = varianceIt.GetIndex()[dim] - m_Radius[dim];
      inputSize[dim] = 2 * m_Radius[dim] + 1;
      }

    // Build the input  region
    InputRegionType inputRegion;
    inputRegion.SetIndex(inputIndex);
    inputRegion.SetSize(inputSize);
    inputRegion.Crop(inputPtr->GetRequestedRegion());

    typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
    NeighborhoodIteratorType neighborIt;

    //initalize lookup array.
    LookupArrayType lookupArray(m_NumberOfBinsPerAxis * m_NumberOfBinsPerAxis);

    lookupArray.Fill(-1);

    GreyLevelCooccurrenceListType glcList;
    TotalAbsoluteFrequencyType totalFrequency = 0;

    const double minSizeHist = std::min (m_Histogram->GetSize()[0], m_Histogram->GetSize()[1]);

    neighborIt = NeighborhoodIteratorType(m_NeighborhoodRadius, inputPtr, inputRegion);
    for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
    {
    const InputPixelType centerPixelIntensity = neighborIt.GetCenterPixel();
    if ( centerPixelIntensity < m_InputImageMinimum
         || centerPixelIntensity > m_InputImageMaximum )
      {
      continue; // don't put a pixel in the histogram if the value
                // is out-of-bounds.
      }

    bool            pixelInBounds;
    const InputPixelType pixelIntensity = neighborIt.GetPixel(m_Offset, pixelInBounds);
    if ( !pixelInBounds )
        {
        continue; // don't put a pixel in the histogram if it's out-of-bounds.
        }
    if ( pixelIntensity < m_InputImageMinimum
         || pixelIntensity > m_InputImageMaximum )
      {
      continue; // don't put a pixel in the histogram if the value
      // is out-of-bounds.
      }

      unsigned int instanceId = 0;
      HistogramIndexType instanceIndex;
      MeasurementVectorType measurement( MeasurementVectorSize );
      measurement[0] = centerPixelIntensity;
      measurement[1] = pixelIntensity;
      //Get Index of the histogram for the given pixel pair;
      m_Histogram->GetIndex(measurement, instanceIndex);
      //Find the 1D index of the historam index. This index is used to check the
      //entry in lookup array.
      instanceId = instanceIndex[1] * m_NumberOfBinsPerAxis + instanceIndex[0];
      if( lookupArray[instanceId] < 0)
        {
        lookupArray[instanceId] = glcList.size();
        CooccurrenceType cooccur;
        cooccur.index = instanceIndex;
        cooccur.frequency = 1;
        glcList.push_back(cooccur);
        }
      else
        {
        int vindex = lookupArray[instanceId];
        glcList[vindex].frequency++;
        }

      //For symmetry store the same pixel pair.
      measurement[1] = centerPixelIntensity;
      measurement[0] = pixelIntensity;
      m_Histogram->GetIndex(measurement, instanceIndex);
      instanceId = instanceIndex[1] * m_NumberOfBinsPerAxis + instanceIndex[0];

      if( lookupArray[instanceId] < 0)
        {
        lookupArray[instanceId] = glcList.size();
        CooccurrenceType cooccur;
        cooccur.index = instanceIndex;
        cooccur.frequency = 1;
        glcList.push_back(cooccur);
        }
      else
        {
        int vindex = lookupArray[instanceId];
        glcList[vindex].frequency = glcList[vindex].frequency + 1;
        }

      // Increment total frequency by two as we consider symmetry of
      // cooccurrence pairs
      totalFrequency = totalFrequency + 2;
    }

    MeasurementType m_Mean            				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_Variance        				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_Dissimilarity    				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_SumAverage      				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_SumEntropy      				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_SumVariance     				= itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_DifferenceEntropy       = itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_DifferenceVariance      = itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_IC1                     = itk::NumericTraits< MeasurementType >::Zero;
    MeasurementType m_IC2                     = itk::NumericTraits< MeasurementType >::Zero;

    double Entropy = 0;

    long unsigned int histSize = m_Histogram->GetSize()[0];
    long unsigned int twiceHistSize = 2 * m_Histogram->GetSize()[0];

    typedef itk::Array<double> DoubleArrayType;
    DoubleArrayType hx(histSize);
    DoubleArrayType hy(histSize);
    DoubleArrayType pdxy(twiceHistSize);

    for(long unsigned int i = 0; i < histSize; i++)
      {
      hx[i] = 0.0;
      hy[i] = 0.0;
      pdxy[i] = 0.0;
      }
    for(long unsigned int i = histSize; i < twiceHistSize; i++)
      {
      pdxy[i] = 0.0;
      }

    /*   hx.Fill(0.0);    hy.Fill(0.0);    pdxy.Fill(0.0);   */
    double hxy1 = 0;

    GreyLevelCooccurrenceListIteratorType vectorIt;
    GreyLevelCooccurrenceListConstIteratorType covectorIt;
    //Normalize the GreyLevelCooccurrenceListType
    //Compute Mean, Entropy (f12), hx, hy, pdxy
    vectorIt = glcList.begin();
    while( vectorIt != glcList.end())
      {
      HistogramIndexType index = (*vectorIt).index;
      double frequency = static_cast<double>((*vectorIt).frequency) / static_cast<double>(totalFrequency);
      m_Mean += static_cast<double>(index[0]) * frequency;
      Entropy -= (frequency > 0.0001) ? frequency * vcl_log(frequency) / log2 : 0.;
      //update normalized frequency
      (*vectorIt).frequency = frequency;

      unsigned int i = index[1];
      unsigned int j = index[0];
      hx[j] += frequency;
      hy[i] += frequency;

      if( i+j > histSize-1)
        {
        pdxy[i+j] += frequency;
        }
      if( i <= j )
        {
        pdxy[j-i] += frequency;
        }

      ++vectorIt;
      }

    //second pass over normalized co-occurrence list to find variance and pipj.
    //pipj is needed to calculate f11
    vectorIt = glcList.begin();
    while( vectorIt != glcList.end())
      {
      double frequency = (*vectorIt).frequency;
      HistogramIndexType index = (*vectorIt).index;
      unsigned int i = index[1];
      unsigned int j = index[0];
      double index0 = static_cast<double>(index[0]);
      m_Variance += ((index0 - m_Mean) * (index0 - m_Mean)) * frequency;
      double pipj = hx[j] * hy[i];
      hxy1 -= (pipj > 0.0001) ? frequency * vcl_log(pipj) : 0.;
      ++vectorIt;
      }

    //iterate histSize to compute sumEntropy
    double PSSquareCumul = 0;
    for(long unsigned int k = histSize; k < twiceHistSize; k++)
      {
           m_SumAverage += k * pdxy[k];
           m_SumEntropy -= (pdxy[k] > 0.0001) ? pdxy[k] * vcl_log(pdxy[k]) / log2 : 0;
           PSSquareCumul += k * k * pdxy[k];
      }
    m_SumVariance = PSSquareCumul - m_SumAverage * m_SumAverage;

    double PDSquareCumul = 0;
    double PDCumul = 0;
    double hxCumul = 0;
    double hyCumul = 0;

    for (long unsigned int i = 0; i < minSizeHist; ++i)
      {
      double pdTmp = pdxy[i];
      PDCumul += i * pdTmp;
      m_DifferenceEntropy -= (pdTmp > 0.0001) ? pdTmp * vcl_log(pdTmp) / log2 : 0;
      PDSquareCumul += i * i * pdTmp;

      //comput hxCumul and hyCumul
      double marginalfreq = hx[i];
      hxCumul += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;

      marginalfreq = hy[i];
      hyCumul += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;
      }
    m_DifferenceVariance = PDSquareCumul - PDCumul * PDCumul;

    /* pipj computed below is totally different from earlier one which was used
     * to compute hxy1. This need to force an iterator over entire histogram.
       Processing time is propotional to the histogram bin size */
    double hxy2 = 0;
		for(int i = 0; i < histSize; ++i)
      {
      for(int j = 0; j < histSize; ++j)
        {
        double pipj = hx[j] * hy[i];
        hxy2 -= (pipj > 0.0001) ? pipj * vcl_log(pipj) : 0.;
        double frequency = 0;
        int instanceId = i * m_Histogram->GetSize()[0] + j;
        if (instanceId < lookupArray.size())
          {
          int findex = lookupArray[instanceId];
          if(findex > -1 )
            frequency = glcList[findex].frequency;
          }
        m_Dissimilarity+= ( j - i ) * (frequency * frequency);
        }
      }

    //Information measures of correlation 1 & 2
    m_IC1 = (vcl_abs(std::max (hxCumul, hyCumul)) > 0.0001) ? (Entropy - hxy1) / (std::max (hxCumul, hyCumul)) : 0;
    m_IC2 = 1 - vcl_exp (-2. * vcl_abs (hxy2 - Entropy));
    m_IC2 = (m_IC2 >= 0) ? vcl_sqrt (m_IC2) : 0;

    // Fill outputs
    meanIt.Set(m_Mean);
    varianceIt.Set(m_Variance);
    dissimilarityIt.Set(m_Dissimilarity);
    sumAverageIt.Set(m_SumAverage);
    sumVarianceIt.Set(m_SumVariance);
    sumEntropytIt.Set(m_SumEntropy);
    differenceEntropyIt.Set(m_DifferenceEntropy);
    differenceVarianceIt.Set(m_DifferenceVariance);
    ic1It.Set(m_IC1);
    ic2It.Set(m_IC2);

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    ++varianceIt;
    ++meanIt;
    ++dissimilarityIt;
    ++sumAverageIt;
    ++sumVarianceIt;
    ++sumEntropytIt;
    ++differenceEntropyIt;
    ++differenceVarianceIt;
    ++ic1It;
    ++ic2It;
    }

}

} // End namespace otb

#endif
