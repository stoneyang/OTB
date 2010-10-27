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

#ifndef __otbSarParametricMapFunction_txx
#define __otbSarParametricMapFunction_txx

#include "otbSarParametricMapFunction.h"
#include "itkNumericTraits.h"


#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarParametricMapFunction<TInputImage, TCoordRep>
::SarParametricMapFunction()
{
  m_PointSet = PointSetType::New();
  m_Coeff    = PointSetType::New();
  m_IsInitialize = false;
  m_UsingClosestPointMethod = false;
}

template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::SetConstantValue(const RealType& value)
{
  PointType  p0;
  p0.Fill(0);
  m_IsInitialize = false;
  m_PointSet->Initialize();
  m_PointSet->SetPoint(0, p0);
  m_PointSet->SetPointData(0, value);
  EvaluateParametricCoefficient();
  this->Modified();
}


template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::SetPolynomalSize(const IndexType PolynomalSize)
{
  typedef typename IndexType::IndexValueType IndexValueType;
  IndexValueType pointId = 0;
  PointType coef;
  coef.Fill(0);

  m_IsInitialize = false;
  for (IndexValueType i = 0; i <= PolynomalSize[0]; ++i)
    {
    coef[0] = i;
    for (IndexValueType j = 0; j <= PolynomalSize[1]; ++j)
      {
      coef[1] = j;
      m_Coeff->SetPoint(pointId, coef);
      ++pointId;
      }
    }
  if (m_PointSet->GetNumberOfPoints() > 0)
    {
    EvaluateParametricCoefficient();
    }
  this->Modified();
}

template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::EvaluateParametricCoefficient()
{
  PointSetPointer pointSet;
  pointSet = this->GetPointSet();

  if (pointSet->GetNumberOfPoints() == 0)
    {
    itkExceptionMacro(<< "PointSet must be set before evaluating the parametric coefficient (at least one value)");
    }

  PointType  coef;
  PointType  point;
  coef.Fill(0);
  point.Fill(0);

  typename PointSetType::PixelType pointValue;
  pointValue = itk::NumericTraits<PixelType>::Zero;

  if(pointSet->GetNumberOfPoints() == 1)
    {
    coef[0] = 0;
    coef[1] = 0;
    m_Coeff->SetPoint(0,coef);
    pointSet->GetPointData(0, &pointValue);
    m_Coeff->SetPointData(0,pointValue);
    }

  // Perform the plane least square estimation
  unsigned int nbRecords = pointSet->GetNumberOfPoints();
  unsigned int nbCoef    = m_Coeff->GetNumberOfPoints();

  vnl_sparse_matrix<double> a(nbRecords, nbCoef);
  vnl_vector<double> b(nbRecords), bestParams(nbCoef);

  // Fill the linear system
  for (unsigned int i = 0; i < nbRecords; ++i)
    {
    this->GetPointSet()->GetPoint(i, &point);
    this->GetPointSet()->GetPointData(i, &pointValue);
    b(i)  = pointValue;

    for(unsigned int pointId = 0; pointId < nbCoef; ++pointId)
      {
      PointType  powerCoef;
      powerCoef.Fill(0);
      this->GetCoeff()->GetPoint(pointId, &powerCoef);
      a(i,pointId)  = vcl_pow(point[0],powerCoef[0]);
      a(i,pointId) *= vcl_pow(point[1],powerCoef[1]);
      }
    }

  if(this->GetCoeff()->GetNumberOfPoints()>0)
  {
  // Create the linear system
  vnl_sparse_matrix_linear_system<double> linearSystem(a, b);
  vnl_lsqr linearSystemSolver(linearSystem);

  // And solve it
  if(this->GetCoeff()->GetNumberOfPoints()>1)
	linearSystemSolver.minimize(bestParams);

  for(unsigned int pointId = 0; pointId < nbCoef; ++pointId)
    {
    this->GetCoeff()->SetPointData(pointId,bestParams[pointId]);
  }
    }
  m_IsInitialize = true;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarParametricMapFunction<TInputImage, TCoordRep>
::RealType
SarParametricMapFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType result;
  typename PointSetType::PixelType pointValue;
  pointValue = itk::NumericTraits<PixelType>::Zero;

  result = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<RealType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    return (itk::NumericTraits<RealType>::max());
    }

  if (m_IsInitialize == false )
    {
    itkExceptionMacro(<< "must estimate parameters before evaluating ");
    }

  if(m_UsingClosestPointMethod == false )
    {
    for(unsigned int pointId = 0; pointId < m_Coeff->GetNumberOfPoints(); ++pointId)
      {
      PointType  powerCoef;
      powerCoef.Fill(0);

      this->GetCoeff()->GetPoint(pointId, &powerCoef);
      this->GetCoeff()->GetPointData(pointId, &pointValue);

      result += pointValue * vcl_pow(index[0],powerCoef[0]) * vcl_pow(index[1],powerCoef[1]);
      }
    }

  return result;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
SarParametricMapFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  PointType  point;
  point.Fill(0);
  PixelType pointValue;
  pointValue = itk::NumericTraits<PixelType>::Zero;

  this->Superclass::PrintSelf(os, indent);

  for(unsigned int i = 0; i < m_Coeff->GetNumberOfPoints(); ++i)
  {
    m_Coeff->GetPoint(i,&point);
    m_Coeff->GetPointData(i,&pointValue);
    os << indent << "Polynom coefficient: "  << point <<" with value : "<< pointValue << std::endl;
  }

}


} // end namespace otb

#endif
