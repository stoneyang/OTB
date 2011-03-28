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
#ifndef __otbWrapperChoiceParameter_h
#define __otbWrapperChoiceParameter_h

#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class ChoiceParameter
 *  \brief This class represent a multiple choice parameter for the wrapper framework
 *
 *  Represents an enumerated parameter.
 *  The different values of the enumeration can be associated with a parameter
 */
class ChoiceParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef ChoiceParameter               Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ChoiceParameter, Parameter);

  /** Add a value to the choice */
  void AddChoice( std::string key, Parameter::Pointer param )
  {
    m_ChoiceMap[key] = param;
  }

protected:
  /** Constructor */
  ChoiceParameter()
  {}

  /** Destructor */
  ~ChoiceParameter()
  {}

private:
  ChoiceParameter(const ChoiceParameter &); //purposely not implemented
  void operator =(const ChoiceParameter&); //purposely not implemented

  typedef std::map<std::string, Parameter::Pointer> ChoiceMap;
  ChoiceMap m_ChoiceMap;



}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
