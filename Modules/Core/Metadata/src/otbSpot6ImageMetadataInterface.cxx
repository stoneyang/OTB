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

#include "otbSpot6ImageMetadataInterface.h"

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include <boost/lexical_cast.hpp>


namespace otb
{
using boost::lexical_cast;
using boost::bad_lexical_cast;

Spot6ImageMetadataInterface
::Spot6ImageMetadataInterface()
{
}

bool
Spot6ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("SPOT 6") != std::string::npos)
    return true;
  else
    return false;
}

std::string
Spot6ImageMetadataInterface::GetInstrument() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (imageKeywordlist.HasKey("support_data.instrument"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument");
    return valueString;
    }

  return "";
}

std::string
Spot6ImageMetadataInterface::GetInstrumentIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (imageKeywordlist.HasKey("support_data.instrument_index"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument_index");
    return valueString;
    }

  return ""; // Invalid value
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.solar_irradiance"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.solar_irradiance");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
      }
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Irradiance");
    }

  return outputValuesVariableLengthVector;
}


int
Spot6ImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[2]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Day");
    }

  return value;
}

int
Spot6ImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[1]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Month");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[0]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[3]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Hour");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[4]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Minute");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[2]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Day");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[1]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Month");
    }
  return value;
}

int
Spot6ImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[0]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}


Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.physical_bias"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.physical_bias");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
      }
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Bias");
    }

  return outputValuesVariableLengthVector;
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.physical_gain"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.physical_gain");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
      }
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Bias");
    }

  return outputValuesVariableLengthVector;
}


double
Spot6ImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.incident_angle"))
    {
    return 0;
    }

  // MSD: for the moment take only topCenter value
  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.incident_angle");
  double value = atof(valueString.c_str());
  return value;
}

double
Spot6ImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.scene_orientation"))
    {
    return 0;
    }

  // MSD: for the moment take only topCenter value
  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");
  double satAz = atof(valueString.c_str());

  return satAz;
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  int         nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    wavel.Fill(0.455); // todo
    }
  else if (nbBands > 1 && nbBands < 5) // todo
    {
    wavel.SetSize(4);
    wavel[0] = 0.454;
    wavel[1] = 0.527;
    wavel[2] = 0.624;
    wavel[3] = 0.756;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
           itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  int nbBands = this->GetNumberOfBands();

  // Panchromatic case
  if (nbBands == 1) // todo
    {
    wavel.SetSize(1);
    wavel.Fill(0.744);
    }
  else if (nbBands > 1 && nbBands < 5) // todo
    {
    wavel.SetSize(4);
    wavel[0] = 0.519;
    wavel[1] = 0.587;
    wavel[2] = 0.694;
    wavel[3] = 0.880;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

// TODO MSD need to update this function
// Comment this part as relative response
// FIXME check if this is coherent with other sensor
unsigned int
Spot6ImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const // todo
{
  int nbBands = this->GetNumberOfBands();
  //Panchromatic case
  if (nbBands == 1)
    {
    return 0;
    }
  else
    {
    otbMsgDevMacro(<< "Spot6 detected: first file component is red band and third component is blue one");
    if (i == 0) return 2;
    if (i == 2) return 0;
    }

  return i;
}

std::vector<std::string>
Spot6ImageMetadataInterface
::GetEnhancedBandNames() const // todo
{
  std::vector<std::string> enhBandNames;
  std::vector<std::string> rawBandNames = this->Superclass::GetBandName();

  if(rawBandNames.size())
    {
    for (std::vector<std::string>::iterator it = rawBandNames.begin(); it != rawBandNames.end(); ++it)
      {
      // Manage Panchro case
      if ( (rawBandNames.size() == 1) && !(*it).compare("P") )
        {
        enhBandNames.push_back("PAN");
        break;
        }
      else if ((rawBandNames.size() != 1) && !(*it).compare("P"))
        {
        /* Launch exception situation not valid*/
        itkExceptionMacro(<< "Invalid Metadata, we cannot provide an consistent name to the band");
        }

      // Manage MS case
      if ( !(*it).compare("B0") )
        {
        enhBandNames.push_back("Blue");
        }
      else if ( !(*it).compare("B1") )
        {
        enhBandNames.push_back("Green");
        }
      else if ( !(*it).compare("B2") )
        {
        enhBandNames.push_back("Red");
        }
      else if ( !(*it).compare("B3") )
        {
        enhBandNames.push_back("NIR");
        }
      else
        {
        enhBandNames.push_back("Unknown");
        }
      }
    }

  return enhBandNames;

}

std::vector<unsigned int>
Spot6ImageMetadataInterface
::GetDefaultDisplay() const // todo
{

  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 1;
  rgb[2] = 2;
  return rgb;

}

Spot6ImageMetadataInterface::WavelengthSpectralBandVectorType
Spot6ImageMetadataInterface
::GetSpectralSensitivity()  const // todo
{
  //TODO tabulate spectral responses
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::list <std::vector<float> > tmpSpectralBandList;

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  const int nbBands = this->GetNumberOfBands();
  const std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    //if (sensorId.find("PHR") != std::string::npos)
    if (sensorId == "SPOT 6") // todo
      {
      const float pan[601] =
        {
0.00007493,
0.00008750,
0.00009848,
0.00010843,
0.00011646,
0.00012259,
0.00012552,
0.00012814,
0.00013531,
0.00015014,
0.00017236,
0.00019564,
0.00021235,
0.00022348,
0.00022676,
0.00022299,
0.00021213,
0.00020083,
0.00019926,
0.00021359,
0.00025083,
0.00030983,
0.00038670,
0.00047229,
0.00056950,
0.00067560,
0.00078669,
0.00090069,
0.00103421,
0.00131540,
0.00169029,
0.00228198,
0.00312648,
0.00417071,
0.00533582,
0.00661006,
0.00802817,
0.00960467,
0.01132980,
0.01334611,
0.01609231,
0.02038299,
0.02732543,
0.03802347,
0.05294767,
0.07167096,
0.09348728,
0.11872659,
0.14830802,
0.18269957,
0.22269040,
0.26906796,
0.32263521,
0.38325366,
0.44824011,
0.51559417,
0.58258589,
0.64645021,
0.70236661,
0.74583583,
0.77585237,
0.79433792,
0.80499388,
0.81090952,
0.81371638,
0.81431418,
0.81316313,
0.81141755,
0.81097193,
0.81322836,
0.81878063,
0.82800769,
0.83916423,
0.85084085,
0.86161004,
0.87040692,
0.87637562,
0.87967534,
0.88047879,
0.88112137,
0.88175192,
0.88365397,
0.88759508,
0.89291809,
0.89912245,
0.90543957,
0.91146456,
0.91547923,
0.91843965,
0.91963891,
0.91975983,
0.91963913,
0.91947516,
0.92025788,
0.92260193,
0.92575482,
0.93070662,
0.93550391,
0.94001389,
0.94383039,
0.94609629,
0.94688618,
0.94624211,
0.94455744,
0.94265988,
0.94120799,
0.94079078,
0.94176327,
0.94473408,
0.94915584,
0.95419339,
0.95954236,
0.96459688,
0.96902841,
0.97179112,
0.97317857,
0.97320070,
0.97218736,
0.97091309,
0.97035732,
0.96996616,
0.97140258,
0.97376942,
0.97636229,
0.97957895,
0.98311659,
0.98550114,
0.98688700,
0.98705275,
0.98569258,
0.98301588,
0.97915984,
0.97493690,
0.97129138,
0.96813872,
0.96649466,
0.96685400,
0.96849224,
0.97201807,
0.97702694,
0.98197923,
0.98753229,
0.99230951,
0.99540351,
0.99740084,
0.99730413,
0.99558763,
0.99262103,
0.98803927,
0.98368879,
0.97966358,
0.97601679,
0.97409076,
0.97356455,
0.97460306,
0.97733165,
0.98142794,
0.98607299,
0.99104944,
0.99556304,
0.99892911,
1.00128281,
1.00206838,
1.00124192,
0.99879621,
0.99466280,
0.99032401,
0.98568114,
0.98102690,
0.97756954,
0.97495949,
0.97366152,
0.97436348,
0.97596613,
0.97930722,
0.98307748,
0.98726114,
0.99154671,
0.99533904,
0.99803299,
0.99950550,
1.00000000,
0.99848218,
0.99593416,
0.99288307,
0.98834389,
0.98366197,
0.97971238,
0.97601117,
0.97307044,
0.97155305,
0.97102663,
0.97152475,
0.97341779,
0.97585074,
0.97896590,
0.98255796,
0.98543794,
0.98789503,
0.98994151,
0.99059899,
0.99029895,
0.98922305,
0.98676544,
0.98389430,
0.98067297,
0.97675508,
0.97324400,
0.97055473,
0.96803100,
0.96629732,
0.96561899,
0.96564173,
0.96639591,
0.96771777,
0.96934976,
0.97124886,
0.97291816,
0.97432735,
0.97530655,
0.97542227,
0.97463623,
0.97308990,
0.97065297,
0.96750332,
0.96379792,
0.95963369,
0.95522586,
0.95099476,
0.94711647,
0.94373897,
0.94086191,
0.93909609,
0.93751312,
0.93716161,
0.93749275,
0.93844552,
0.93992284,
0.94157292,
0.94327292,
0.94494878,
0.94621266,
0.94689150,
0.94695971,
0.94601526,
0.94485162,
0.94216490,
0.93936780,
0.93516770,
0.93117984,
0.92600825,
0.92184558,
0.91679022,
0.91281220,
0.90873714,
0.90559995,
0.90280144,
0.90151379,
0.90018969,
0.90026306,
0.90055224,
0.90169504,
0.90303974,
0.90479165,
0.90660298,
0.90788333,
0.90928294,
0.90961910,
0.91044105,
0.90941307,
0.90884628,
0.90648792,
0.90426184,
0.90090015,
0.89728707,
0.89360311,
0.88921759,
0.88536863,
0.88109823,
0.87784434,
0.87367129,
0.87100044,
0.86849439,
0.86645265,
0.86555715,
0.86486409,
0.86444275,
0.86462554,
0.86421244,
0.86509991,
0.86504142,
0.86574075,
0.86547448,
0.86537728,
0.86481040,
0.86329140,
0.86242059,
0.85986818,
0.85807444,
0.85502168,
0.85224705,
0.84852762,
0.84517422,
0.84157685,
0.83807810,
0.83443274,
0.83120263,
0.82819085,
0.82520041,
0.82259510,
0.82050631,
0.81863236,
0.81673157,
0.81518962,
0.81376687,
0.81276571,
0.81111987,
0.81039972,
0.80884166,
0.80742716,
0.80605476,
0.80328088,
0.80094601,
0.79797843,
0.79448406,
0.79079478,
0.78664955,
0.78403080,
0.77837404,
0.77671056,
0.77071265,
0.76860809,
0.76568459,
0.76096821,
0.75943556,
0.75440145,
0.74853854,
0.73756232,
0.72050194,
0.69619742,
0.66466129,
0.62570237,
0.58042003,
0.52946690,
0.47429123,
0.41572304,
0.35577781,
0.29694274,
0.24182875,
0.19245049,
0.15026362,
0.11482153,
0.08854978,
0.06698482,
0.05109390,
0.03916000,
0.03018623,
0.02342899,
0.01832614,
0.01444326,
0.01147024,
0.00918513,
0.00740254,
0.00609562,
0.00491959,
0.00435297,
0.00335678,
0.00291823,
0.00234224,
0.00205113,
0.00167084,
0.00145104,
0.00124552,
0.00104170,
0.00108470,
0.00077931,
0.00084497,
0.00059188,
0.00063835,
0.00045747,
0.00040248,
0.00043271,
0.00031792,
0.00033871,
0.00025452,
0.00026931,
0.00020572,
0.00018559,
0.00019577,
0.00015320,
0.00016086,
0.00012704,
0.00011590,
0.00012165,
0.00009829,
0.00010178,
0.00008372,
0.00007722,
0.00008098,
0.00007448,
0.00006262,
0.00006513,
0.00005514,
0.00005663,
0.00004857,
0.00004996,
0.00004369,
0.00004402,
0.00003879,
0.00003626,
0.00003796,
0.00003303,
0.00003435,
0.00003053,
0.00003118,
0.00002826,
0.00002628,
0.00002754,
0.00002484,
0.00002559,
0.00002406,
0.00002348,
0.00002237,
0.00002233,
0.00002269,
0.00002170,
0.00001996,
0.00002062,
0.00001880,
0.00002003,
0.00001893,
0.00001935,
0.00001823,
0.00001794,
0.00001879,
0.00001726,
0.00001797,
0.00001774,
0.00001520,
0.00001784,
0.00001500,
0.00001790,
0.00001464,
0.00001317,
0.00001633,
0.00001127,
0.00001471,
0.00001314,
0.00001114,
0.00000968,
0.00001262,
0.00001126,
0.00002112,
0.00001135,
0.00001674,
0.00001146,
0.00001572,
0.00002157,
0.00001166,
0.00002111,
0.00001206,
0.00002362,
0.00002815,
0.00001740,
0.00001816,
0.00002519,
0.00001627,
0.00002405,
0.00002027,
0.00002469,
0.00002124,
0.00001758,
0.00002971,
0.00002051,
0.00002437,
0.00002119,
0.00001926,
0.00001850,
0.00001774,
0.00002056,
0.00002202,
0.00001602,
0.00001436,
0.00001710,
0.00001369,
0.00001827,
0.00001218,
0.00001567,
0.00001596,
0.00001348,
0.00001857,
0.00001662,
0.00001805,
0.00002361,
0.00001927,
0.00003019,
0.00003036,
0.00003602,
0.00003469,
0.00004530,
0.00004918,
0.00005892,
0.00005708,
0.00006488,
0.00006319,
0.00007284,
0.00006971,
0.00007822,
0.00007143,
0.00007223,
0.00007024,
0.00007097,
0.00007120,
0.00006619,
0.00006294,
0.00005778,
0.00005333,
0.00005405,
0.00004915,
0.00004779,
0.00004379,
0.00004087,
0.00003311,
0.00003608,
0.00003592,
0.00002213,
0.00002159,
0.00002350,
0.00001705,
0.00002055,
0.00001764,
0.00001453,
0.00001636,
0.00001197,
0.00001123,
0.00001127,
0.00001042,
0.00001021,
0.00001099,
0.00001171,
0.00001030,
0.00001236,
0.00001192,
0.00000823,
0.00000820,
0.00000893,
0.00001115,
0.00001001,
0.00000802,
0.00000976,
0.00000966,
0.00000957,
0.00000726,
0.00000967,
0.00001001,
0.00000803,
0.00000726,
0.00000739,
0.00000845,
0.00000698,
0.00000568,
0.00000472,
0.00000650,
0.00000706,
0.00000546,
0.00000433,
0.00000607,
0.00000408,
0.00000788,
0.00000464,
0.00000958,
0.00000689,
0.00000976,
0.00000672,
0.00000574,
0.00000980,
0.00000503,
0.00000796,
0.00000325,
0.00000636,
0.00000334,
0.00000562,
0.00000413,
0.00000400,
0.00000610,
0.00000240,
0.00000486,
0.00000262,
0.00000333,
0.00000152,
0.00000236,
0.00000152,
0.00000228,
0.00000070,
0.00000185,
0.00000162,
0.00000093,
0.00000188,
0.00000116,
0.00000041,
0.00000126
        };
      //add panchromatic band to the temporary list
      const std::vector<float> vpan (pan, pan + sizeof(pan) / sizeof(float) );
      tmpSpectralBandList.push_back(vpan);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Spot6 Sensor ID");
      }
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    if (sensorId == "SPOT 6") // todo
      {
      //band B0 (blue band)
      const float b0[601] =
        {
0.00006627,
0.00006114,
0.00005964,
0.00006755,
0.00007354,
0.00007335,
0.00007303,
0.00006988,
0.00006669,
0.00006467,
0.00008891,
0.00010731,
0.00006420,
0.00008781,
0.00008978,
0.00008292,
0.00008193,
0.00005499,
0.00008203,
0.00011262,
0.00009092,
0.00010575,
0.00013926,
0.00016613,
0.00019666,
0.00019375,
0.00021841,
0.00033008,
0.00059283,
0.00096710,
0.00120426,
0.00143132,
0.00190776,
0.00246548,
0.00314995,
0.00393858,
0.00502501,
0.00693758,
0.00995331,
0.01395395,
0.01816332,
0.02248784,
0.02806060,
0.03582005,
0.04774668,
0.06795621,
0.09842750,
0.13108997,
0.16221496,
0.20486488,
0.26488470,
0.33784410,
0.41558626,
0.48567541,
0.54882896,
0.61466150,
0.68475833,
0.75228611,
0.80736646,
0.84246227,
0.85754473,
0.85841741,
0.85407547,
0.85129313,
0.85144931,
0.85321674,
0.85405128,
0.85387063,
0.85555834,
0.86252514,
0.87558536,
0.89251130,
0.90900546,
0.92154349,
0.92918777,
0.93306769,
0.93515923,
0.93666027,
0.93739349,
0.93661481,
0.93379608,
0.92929650,
0.92442581,
0.92058032,
0.91895381,
0.92090421,
0.92631122,
0.93455782,
0.94463030,
0.95473194,
0.96364502,
0.97043568,
0.97436437,
0.97495098,
0.97200508,
0.96636029,
0.95962307,
0.95371958,
0.95061152,
0.95192705,
0.95818895,
0.96856416,
0.98070274,
0.99142860,
0.99788650,
1.00000000,
0.99910819,
0.99808156,
0.99650360,
0.99314712,
0.98404465,
0.96550845,
0.93867401,
0.90804427,
0.87480177,
0.83387151,
0.77606904,
0.69859059,
0.60871934,
0.51759429,
0.43125686,
0.34977809,
0.27392459,
0.20473488,
0.14738691,
0.10459452,
0.07555179,
0.05654570,
0.04380857,
0.03477386,
0.02793581,
0.02225302,
0.01751704,
0.01359022,
0.01046026,
0.00814691,
0.00649078,
0.00530930,
0.00446096,
0.00383097,
0.00333164,
0.00292664,
0.00257637,
0.00225949,
0.00198202,
0.00172287,
0.00150439,
0.00130359,
0.00113903,
0.00099076,
0.00086925,
0.00076142,
0.00067853,
0.00060841,
0.00055953,
0.00051880,
0.00049699,
0.00047445,
0.00045863,
0.00044229,
0.00042470,
0.00040647,
0.00038780,
0.00035963,
0.00033330,
0.00029660,
0.00027179,
0.00024451,
0.00022875,
0.00021524,
0.00021255,
0.00021004,
0.00021079,
0.00021138,
0.00021521,
0.00021336,
0.00021459,
0.00020993,
0.00020747,
0.00019888,
0.00019065,
0.00018249,
0.00017164,
0.00016473,
0.00015826,
0.00015437,
0.00015378,
0.00015002,
0.00015089,
0.00015050,
0.00015245,
0.00015136,
0.00015264,
0.00015147,
0.00015009,
0.00014792,
0.00014493,
0.00014083,
0.00013665,
0.00013056,
0.00012700,
0.00012212,
0.00011620,
0.00011384,
0.00010918,
0.00010782,
0.00010377,
0.00010231,
0.00010126,
0.00010021,
0.00009925,
0.00009923,
0.00009906,
0.00009850,
0.00009892,
0.00009742,
0.00009802,
0.00009809,
0.00009662,
0.00009692,
0.00009575,
0.00009546,
0.00009575,
0.00009469,
0.00009485,
0.00009453,
0.00009384,
0.00009463,
0.00009451,
0.00009458,
0.00009520,
0.00009478,
0.00009588,
0.00009579,
0.00009545,
0.00009560,
0.00009549,
0.00009549,
0.00009551,
0.00009535,
0.00009579,
0.00009570,
0.00009544,
0.00009552,
0.00009622,
0.00009645,
0.00009648,
0.00009799,
0.00009797,
0.00009864,
0.00009916,
0.00010183,
0.00010153,
0.00010358,
0.00010461,
0.00010712,
0.00010718,
0.00011085,
0.00011321,
0.00011173,
0.00011398,
0.00011982,
0.00011831,
0.00012135,
0.00012902,
0.00012720,
0.00013658,
0.00013538,
0.00014397,
0.00014356,
0.00015119,
0.00015202,
0.00015693,
0.00015686,
0.00015130,
0.00015184,
0.00013690,
0.00013092,
0.00013335,
0.00011361,
0.00010715,
0.00010823,
0.00009932,
0.00009856,
0.00009765,
0.00009914,
0.00010008,
0.00010777,
0.00010686,
0.00011218,
0.00012769,
0.00012456,
0.00014074,
0.00014680,
0.00014400,
0.00015360,
0.00015552,
0.00015166,
0.00015221,
0.00014625,
0.00014561,
0.00013613,
0.00013172,
0.00013242,
0.00012141,
0.00012300,
0.00011836,
0.00010821,
0.00010974,
0.00010309,
0.00010302,
0.00010127,
0.00009629,
0.00009854,
0.00009280,
0.00009475,
0.00009054,
0.00009196,
0.00008822,
0.00008866,
0.00008744,
0.00008550,
0.00008673,
0.00008483,
0.00008628,
0.00008449,
0.00009254,
0.00008532,
0.00009594,
0.00008917,
0.00009084,
0.00010183,
0.00009128,
0.00010000,
0.00008800,
0.00009430,
0.00008055,
0.00008475,
0.00007796,
0.00007850,
0.00007713,
0.00007770,
0.00007623,
0.00007664,
0.00007474,
0.00007405,
0.00007519,
0.00007318,
0.00007349,
0.00007204,
0.00007375,
0.00007369,
0.00007149,
0.00007148,
0.00007312,
0.00007167,
0.00007255,
0.00007120,
0.00007186,
0.00007087,
0.00007145,
0.00007024,
0.00007096,
0.00007041,
0.00007088,
0.00006995,
0.00007149,
0.00006924,
0.00007103,
0.00006949,
0.00007093,
0.00006951,
0.00007194,
0.00007193,
0.00006859,
0.00006781,
0.00007056,
0.00006741,
0.00006870,
0.00006628,
0.00006687,
0.00006601,
0.00006595,
0.00006670,
0.00006450,
0.00006595,
0.00006433,
0.00006569,
0.00006431,
0.00006573,
0.00006406,
0.00006507,
0.00006298,
0.00006428,
0.00006201,
0.00006357,
0.00006101,
0.00006096,
0.00006309,
0.00006103,
0.00006314,
0.00006382,
0.00006164,
0.00006245,
0.00006061,
0.00005973,
0.00006261,
0.00005982,
0.00006197,
0.00005956,
0.00006099,
0.00005881,
0.00005972,
0.00006005,
0.00005691,
0.00005647,
0.00005823,
0.00005603,
0.00005759,
0.00005605,
0.00005558,
0.00005646,
0.00005480,
0.00005543,
0.00005551,
0.00005357,
0.00005295,
0.00005549,
0.00005286,
0.00005495,
0.00005278,
0.00005414,
0.00005475,
0.00005169,
0.00005307,
0.00005117,
0.00005068,
0.00005036,
0.00005270,
0.00004991,
0.00005177,
0.00005900,
0.00004889,
0.00006116,
0.00005285,
0.00006427,
0.00005589,
0.00006943,
0.00006195,
0.00007219,
0.00007275,
0.00006694,
0.00007232,
0.00006712,
0.00007044,
0.00006607,
0.00007040,
0.00007120,
0.00006047,
0.00006863,
0.00006547,
0.00005919,
0.00006852,
0.00005996,
0.00006589,
0.00006938,
0.00006013,
0.00007365,
0.00006702,
0.00007595,
0.00008089,
0.00006987,
0.00008784,
0.00007751,
0.00009504,
0.00010978,
0.00008984,
0.00012777,
0.00010534,
0.00014784,
0.00012341,
0.00017455,
0.00018601,
0.00015342,
0.00020484,
0.00017984,
0.00018931,
0.00022557,
0.00022780,
0.00021393,
0.00022939,
0.00023016,
0.00021920,
0.00021508,
0.00022046,
0.00020159,
0.00021016,
0.00018988,
0.00019842,
0.00017117,
0.00018205,
0.00015240,
0.00014443,
0.00015946,
0.00014629,
0.00012207,
0.00013127,
0.00010545,
0.00011422,
0.00009446,
0.00009843,
0.00008316,
0.00008914,
0.00008435,
0.00006511,
0.00007269,
0.00006160,
0.00006279,
0.00005814,
0.00005882,
0.00005406,
0.00005351,
0.00005183,
0.00005084,
0.00004786,
0.00004877,
0.00004859,
0.00004583,
0.00004279,
0.00004263,
0.00004130,
0.00004093,
0.00004059,
0.00004012,
0.00004037,
0.00004396,
0.00004304,
0.00004054,
0.00004200,
0.00004073,
0.00004199,
0.00004077,
0.00004125,
0.00003820,
0.00003723,
0.00003610,
0.00003503,
0.00003232,
0.00003214,
0.00003263,
0.00003065,
0.00003127,
0.00002955,
0.00002956,
0.00002840,
0.00002896,
0.00002780,
0.00002805,
0.00002680,
0.00002585,
0.00002704,
0.00002540,
0.00002576,
0.00002438,
0.00002409,
0.00002110,
0.00002288,
0.00001930,
0.00002147,
0.00001830,
0.00001907,
0.00001917,
0.00001634,
0.00001778,
0.00001705,
0.00001506,
0.00001555,
0.00001544,
0.00001376,
0.00001432,
0.00001256,
0.00001321,
0.00001143,
0.00001207,
0.00001040,
0.00001079,
0.00001077,
0.00000938,
0.00000992,
0.00000926,
0.00000875,
0.00000915,
0.00000791,
0.00000858,
0.00000757,
0.00000885,
0.00000740,
0.00000859,
0.00000871,
0.00000732,
0.00000711
        };
      //B1 green band
      const float b1[601] =
        {
0.00001010,
0.00001389,
0.00001902,
0.00001356,
0.00001327,
0.00002873,
0.00003122,
0.00002184,
0.00002860,
0.00002274,
0.00006723,
0.00008343,
0.00001226,
0.00006171,
0.00005563,
0.00004426,
0.00004539,
0.00000677,
0.00002035,
0.00001522,
0.00000528,
0.00001865,
0.00001834,
0.00002028,
0.00001608,
0.00000017,
0.00000057,
0.00001313,
0.00001658,
0.00000224,
0.00001983,
0.00001885,
0.00001859,
0.00001975,
0.00000023,
0.00000139,
0.00001634,
0.00001789,
0.00000616,
0.00002267,
0.00001903,
0.00001956,
0.00001844,
0.00000342,
0.00001448,
0.00001340,
0.00000774,
0.00001880,
0.00001632,
0.00001912,
0.00001915,
0.00000950,
0.00000865,
0.00000927,
0.00001089,
0.00001236,
0.00001349,
0.00001273,
0.00001062,
0.00000958,
0.00000938,
0.00000973,
0.00000998,
0.00001014,
0.00001042,
0.00001060,
0.00001115,
0.00001174,
0.00001254,
0.00001338,
0.00001400,
0.00001431,
0.00001481,
0.00001528,
0.00001646,
0.00001812,
0.00001991,
0.00002232,
0.00002548,
0.00002855,
0.00003235,
0.00003705,
0.00004200,
0.00004766,
0.00005414,
0.00006070,
0.00006821,
0.00007648,
0.00008464,
0.00009410,
0.00010524,
0.00011803,
0.00013402,
0.00015319,
0.00017614,
0.00020243,
0.00023094,
0.00026260,
0.00029673,
0.00033499,
0.00038048,
0.00043594,
0.00050952,
0.00061362,
0.00076395,
0.00097908,
0.00126923,
0.00163382,
0.00207590,
0.00261314,
0.00327921,
0.00413303,
0.00526369,
0.00684627,
0.00915649,
0.01267648,
0.01800685,
0.02585007,
0.03667779,
0.05118064,
0.07035894,
0.09617429,
0.13094754,
0.17727190,
0.23641290,
0.30751157,
0.38790047,
0.47436827,
0.56468213,
0.65643377,
0.74674718,
0.82841738,
0.89713046,
0.94858047,
0.98476388,
1.00572477,
1.01799762,
1.02241761,
1.02351216,
1.02270186,
1.02115905,
1.01965328,
1.01731773,
1.01540955,
1.01129946,
1.00955329,
1.00573235,
1.00407549,
1.00164097,
1.00130077,
0.99962912,
1.00130006,
1.00140898,
1.00384853,
1.00599723,
1.00910590,
1.01051975,
1.01099594,
1.00890543,
1.00483865,
0.99901144,
0.99220799,
0.98468953,
0.97823873,
0.97314859,
0.97022501,
0.96868569,
0.97004107,
0.97166154,
0.97626613,
0.98067188,
0.98683373,
0.99219054,
0.99666299,
1.00000000,
0.99912543,
0.99696320,
0.99036225,
0.98384547,
0.97519907,
0.96391617,
0.94573906,
0.91096960,
0.85904551,
0.79290657,
0.72361833,
0.65257621,
0.57623116,
0.48870213,
0.39405229,
0.29840580,
0.21347365,
0.14739732,
0.10143634,
0.07106678,
0.05101665,
0.03753145,
0.02803887,
0.02107987,
0.01582745,
0.01178705,
0.00868732,
0.00634606,
0.00461992,
0.00337907,
0.00250694,
0.00189987,
0.00147499,
0.00117574,
0.00096470,
0.00080813,
0.00069267,
0.00060187,
0.00053101,
0.00046977,
0.00041864,
0.00037515,
0.00033197,
0.00029595,
0.00026098,
0.00022974,
0.00020299,
0.00017878,
0.00015870,
0.00014470,
0.00012912,
0.00012281,
0.00011148,
0.00010580,
0.00010508,
0.00009942,
0.00009982,
0.00009664,
0.00009667,
0.00009589,
0.00009628,
0.00009600,
0.00009459,
0.00009484,
0.00009162,
0.00009246,
0.00008883,
0.00008616,
0.00008688,
0.00008318,
0.00008295,
0.00008153,
0.00008262,
0.00008265,
0.00008855,
0.00009224,
0.00008974,
0.00010120,
0.00010537,
0.00010299,
0.00011271,
0.00011213,
0.00011810,
0.00011639,
0.00011824,
0.00011152,
0.00011302,
0.00010361,
0.00010441,
0.00009482,
0.00009519,
0.00008675,
0.00008735,
0.00008078,
0.00008067,
0.00007509,
0.00007486,
0.00006895,
0.00006944,
0.00006375,
0.00006358,
0.00005878,
0.00005896,
0.00005523,
0.00005501,
0.00005343,
0.00005281,
0.00005140,
0.00005170,
0.00005134,
0.00005138,
0.00005107,
0.00005085,
0.00005096,
0.00005018,
0.00005046,
0.00004904,
0.00004833,
0.00004769,
0.00004781,
0.00004694,
0.00004600,
0.00004584,
0.00004562,
0.00004584,
0.00004525,
0.00004562,
0.00004612,
0.00004747,
0.00004854,
0.00004734,
0.00004932,
0.00004894,
0.00005047,
0.00004972,
0.00005030,
0.00005129,
0.00004999,
0.00005048,
0.00004864,
0.00004901,
0.00004923,
0.00004719,
0.00004950,
0.00004776,
0.00005348,
0.00005702,
0.00005214,
0.00006320,
0.00005556,
0.00007144,
0.00005903,
0.00005649,
0.00007807,
0.00004800,
0.00004042,
0.00005618,
0.00002571,
0.00003251,
0.00002175,
0.00000592,
0.00001077,
0.00000286,
0.00000380,
0.00000323,
0.00000149,
0.00000247,
0.00000112,
0.00000145,
0.00000111,
0.00000139,
0.00000105,
0.00000141,
0.00000097,
0.00000085,
0.00000123,
0.00000158,
0.00000089,
0.00000137,
0.00000086,
0.00000158,
0.00000202,
0.00000091,
0.00000174,
0.00000102,
0.00000200,
0.00000128,
0.00000479,
0.00000149,
0.00000719,
0.00000417,
0.00000502,
0.00000963,
0.00000561,
0.00000984,
0.00000949,
0.00000318,
0.00000643,
0.00000153,
0.00000184,
0.00000116,
0.00000173,
0.00000086,
0.00000185,
0.00000089,
0.00000193,
0.00000096,
0.00000196,
0.00000116,
0.00000200,
0.00000115,
0.00000195,
0.00000131,
0.00000197,
0.00000125,
0.00000187,
0.00000099,
0.00000070,
0.00000049,
0.00000119,
0.00000051,
0.00000135,
0.00000086,
0.00000153,
0.00000104,
0.00000186,
0.00000156,
0.00000205,
0.00000293,
0.00000170,
0.00000296,
0.00000182,
0.00000266,
0.00000171,
0.00000236,
0.00000240,
0.00000093,
0.00000174,
0.00000183,
0.00000096,
0.00000144,
0.00000177,
0.00000076,
0.00000198,
0.00000079,
0.00000160,
0.00000058,
0.00000154,
0.00000070,
0.00000161,
0.00000076,
0.00000068,
0.00000245,
0.00000079,
0.00000299,
0.00000128,
0.00000285,
0.00000154,
0.00000296,
0.00000178,
0.00000333,
0.00000130,
0.00000347,
0.00000220,
0.00000159,
0.00000449,
0.00000188,
0.00000635,
0.00000903,
0.00000344,
0.00001051,
0.00000601,
0.00000991,
0.00001299,
0.00000632,
0.00001514,
0.00000941,
0.00001426,
0.00000749,
0.00001524,
0.00000887,
0.00000743,
0.00001404,
0.00000558,
0.00001092,
0.00001135,
0.00000460,
0.00001024,
0.00000547,
0.00001036,
0.00000503,
0.00001140,
0.00000557,
0.00001124,
0.00000612,
0.00001120,
0.00000642,
0.00001093,
0.00000638,
0.00001191,
0.00000531,
0.00000907,
0.00000487,
0.00000794,
0.00000998,
0.00000316,
0.00000322,
0.00001260,
0.00000365,
0.00000446,
0.00001209,
0.00000567,
0.00001173,
0.00000705,
0.00001006,
0.00000466,
0.00000325,
0.00000921,
0.00000755,
0.00000493,
0.00000922,
0.00000673,
0.00001643,
0.00001135,
0.00001160,
0.00002335,
0.00001573,
0.00002364,
0.00001931,
0.00002613,
0.00002318,
0.00001698,
0.00001954,
0.00001127,
0.00001395,
0.00001618,
0.00000862,
0.00001383,
0.00000962,
0.00000696,
0.00000696,
0.00000561,
0.00000807,
0.00000905,
0.00000632,
0.00000850,
0.00000820,
0.00001222,
0.00000921,
0.00001027,
0.00001204,
0.00001323,
0.00001373,
0.00001579,
0.00001521,
0.00002156,
0.00001894,
0.00002375,
0.00002243,
0.00002409,
0.00002019,
0.00002182,
0.00001452,
0.00001755,
0.00001475,
0.00000805,
0.00001071,
0.00000537,
0.00000682,
0.00000663,
0.00000526,
0.00000418,
0.00000477,
0.00000426,
0.00000405,
0.00000367,
0.00000399,
0.00000340,
0.00000339,
0.00000169,
0.00000178,
0.00000242,
0.00000171,
0.00000172,
0.00000233,
0.00000182,
0.00000230,
0.00000204,
0.00000245,
0.00000243,
0.00000136,
0.00000259,
0.00000105,
0.00000239,
0.00000090,
0.00000213,
0.00000104,
0.00000226,
0.00000088,
0.00000229,
0.00000065,
0.00000246,
0.00000063,
0.00000238,
0.00000072,
0.00000210,
0.00000091,
0.00000122,
0.00000184,
0.00000177,
0.00000066,
0.00000153,
0.00000065,
0.00000147,
0.00000054,
0.00000145,
0.00000023,
0.00000134,
0.00000029,
0.00000141,
0.00000045,
0.00000127,
0.00000146,
0.00000040,
0.00000142,
0.00000038
        };
      //B2 red band
      const float b2[601] =
        {
0.00001168,
0.00001419,
0.00001348,
0.00000996,
0.00001009,
0.00001366,
0.00003289,
0.00001748,
0.00002207,
0.00002377,
0.00002383,
0.00002470,
0.00001811,
0.00001353,
0.00001155,
0.00001303,
0.00001374,
0.00001126,
0.00000712,
0.00000464,
0.00000438,
0.00000381,
0.00000312,
0.00000581,
0.00000319,
0.00000473,
0.00000585,
0.00000644,
0.00000967,
0.00000918,
0.00000742,
0.00001090,
0.00001649,
0.00002016,
0.00001631,
0.00001673,
0.00001578,
0.00001766,
0.00002209,
0.00003114,
0.00003091,
0.00001848,
0.00003451,
0.00003492,
0.00004332,
0.00004770,
0.00004658,
0.00004547,
0.00004239,
0.00005155,
0.00005834,
0.00006365,
0.00005824,
0.00004581,
0.00003799,
0.00003584,
0.00003689,
0.00003817,
0.00003951,
0.00004017,
0.00003987,
0.00003754,
0.00003651,
0.00003781,
0.00003645,
0.00003408,
0.00003160,
0.00003251,
0.00003509,
0.00003935,
0.00004494,
0.00005115,
0.00005707,
0.00006145,
0.00006038,
0.00005716,
0.00005354,
0.00004978,
0.00004759,
0.00004735,
0.00005985,
0.00006501,
0.00005803,
0.00006474,
0.00006650,
0.00006908,
0.00006585,
0.00005102,
0.00003616,
0.00002648,
0.00002034,
0.00001645,
0.00001363,
0.00001142,
0.00000950,
0.00000755,
0.00000648,
0.00000560,
0.00000476,
0.00000466,
0.00000444,
0.00000389,
0.00000408,
0.00000399,
0.00000392,
0.00000436,
0.00000413,
0.00000397,
0.00000424,
0.00000400,
0.00000389,
0.00000385,
0.00000390,
0.00000379,
0.00000357,
0.00000383,
0.00000386,
0.00000378,
0.00000439,
0.00000377,
0.00000416,
0.00000494,
0.00000455,
0.00000553,
0.00000605,
0.00000613,
0.00000672,
0.00000696,
0.00000744,
0.00000760,
0.00000766,
0.00000770,
0.00000725,
0.00000682,
0.00000716,
0.00000758,
0.00000759,
0.00000722,
0.00000805,
0.00000826,
0.00000816,
0.00000835,
0.00000798,
0.00000798,
0.00000796,
0.00000817,
0.00000856,
0.00000897,
0.00000920,
0.00000935,
0.00000980,
0.00001003,
0.00000998,
0.00001038,
0.00001044,
0.00001035,
0.00001090,
0.00001143,
0.00001155,
0.00001240,
0.00001322,
0.00001374,
0.00001466,
0.00001507,
0.00001561,
0.00001799,
0.00001806,
0.00001990,
0.00002207,
0.00002400,
0.00002768,
0.00003153,
0.00003519,
0.00003855,
0.00004048,
0.00004339,
0.00004685,
0.00004712,
0.00004885,
0.00005031,
0.00005058,
0.00005380,
0.00006109,
0.00006875,
0.00008360,
0.00010367,
0.00013385,
0.00017220,
0.00021349,
0.00025613,
0.00029745,
0.00033957,
0.00038077,
0.00042014,
0.00045664,
0.00049082,
0.00052462,
0.00057202,
0.00064046,
0.00073788,
0.00088389,
0.00109195,
0.00138377,
0.00179072,
0.00234897,
0.00310035,
0.00409611,
0.00539886,
0.00764596,
0.00982485,
0.01250677,
0.01576553,
0.01971871,
0.02460136,
0.03076162,
0.03870973,
0.04932564,
0.06405112,
0.08527458,
0.11683854,
0.16336997,
0.22811553,
0.30778033,
0.39105868,
0.46939880,
0.54633346,
0.62987089,
0.71765372,
0.80227884,
0.87400338,
0.92669327,
0.95728386,
0.97246242,
0.98093913,
0.98815314,
0.99512479,
1.00000000,
1.00071278,
0.99509151,
0.98322813,
0.96618038,
0.94726377,
0.92911907,
0.91427200,
0.90407257,
0.89868685,
0.89729260,
0.90048309,
0.90619876,
0.91486402,
0.92548194,
0.93716217,
0.95109280,
0.96341044,
0.97756310,
0.98873468,
1.00002059,
1.00784176,
1.01361246,
1.01518655,
1.01365106,
1.00889905,
1.00094470,
0.99098939,
0.97907661,
0.96736515,
0.95491923,
0.94403019,
0.93408742,
0.92588661,
0.91979557,
0.91551236,
0.91345541,
0.91406966,
0.91586446,
0.92112352,
0.92637606,
0.93343259,
0.93977000,
0.94511856,
0.94655018,
0.94482431,
0.93695460,
0.92470622,
0.90515848,
0.87651239,
0.84722733,
0.81626058,
0.77926628,
0.74095072,
0.69853161,
0.65530863,
0.60957078,
0.56408506,
0.51760530,
0.47150814,
0.42482252,
0.37706746,
0.32909071,
0.28093192,
0.23513938,
0.19289082,
0.15584587,
0.12438828,
0.09857760,
0.07759031,
0.06074302,
0.04708451,
0.03618668,
0.02749464,
0.02064713,
0.01533676,
0.01131374,
0.00832392,
0.00613477,
0.00454626,
0.00340798,
0.00258536,
0.00199356,
0.00156305,
0.00125005,
0.00101327,
0.00085049,
0.00071368,
0.00062520,
0.00054959,
0.00049985,
0.00047162,
0.00043891,
0.00042848,
0.00040347,
0.00038227,
0.00035692,
0.00033769,
0.00030789,
0.00029814,
0.00024879,
0.00021170,
0.00018471,
0.00013393,
0.00012509,
0.00007240,
0.00006419,
0.00003891,
0.00003294,
0.00002353,
0.00002711,
0.00001690,
0.00001474,
0.00001598,
0.00001292,
0.00001366,
0.00001329,
0.00001411,
0.00002354,
0.00009563,
0.00012882,
0.00011528,
0.00001687,
0.00015343,
0.00012777,
0.00015354,
0.00024298,
0.00015520,
0.00024274,
0.00012696,
0.00014910,
0.00000979,
0.00000629,
0.00000738,
0.00000483,
0.00000489,
0.00000536,
0.00000458,
0.00000528,
0.00000438,
0.00000475,
0.00000446,
0.00000488,
0.00000456,
0.00000547,
0.00000448,
0.00000478,
0.00000435,
0.00000470,
0.00000425,
0.00000458,
0.00000435,
0.00000380,
0.00000373,
0.00000298,
0.00000390,
0.00000278,
0.00000290,
0.00000277,
0.00000307,
0.00000247,
0.00000231,
0.00000301,
0.00000293,
0.00000319,
0.00000351,
0.00000323,
0.00000339,
0.00000318,
0.00000318,
0.00000396,
0.00000313,
0.00000364,
0.00000310,
0.00000419,
0.00000349,
0.00000396,
0.00000341,
0.00000365,
0.00000362,
0.00000290,
0.00000345,
0.00000289,
0.00000288,
0.00000429,
0.00000323,
0.00000329,
0.00000329,
0.00000455,
0.00000497,
0.00000353,
0.00000556,
0.00000383,
0.00000507,
0.00000482,
0.00000484,
0.00000546,
0.00000374,
0.00000601,
0.00000422,
0.00000512,
0.00000509,
0.00000286,
0.00000637,
0.00000539,
0.00000579,
0.00000462,
0.00000647,
0.00000604,
0.00000432,
0.00000662,
0.00000521,
0.00000680,
0.00001180,
0.00000638,
0.00000805,
0.00000389,
0.00001200,
0.00001134,
0.00000562,
0.00001181,
0.00000868,
0.00001541,
0.00001086,
0.00001222,
0.00000823,
0.00000960,
0.00000871,
0.00001051,
0.00001235,
0.00000864,
0.00001357,
0.00001158,
0.00001522,
0.00001091,
0.00001237,
0.00000928,
0.00000973,
0.00000865,
0.00001333,
0.00000847,
0.00001329,
0.00000740,
0.00000974,
0.00001229,
0.00001128,
0.00000704,
0.00001244,
0.00000915,
0.00001448,
0.00001164,
0.00000861,
0.00001441,
0.00000700,
0.00001461,
0.00001401,
0.00001007,
0.00001695,
0.00002042,
0.00002108,
0.00003064,
0.00003171,
0.00004339,
0.00004171,
0.00005692,
0.00007019,
0.00007121,
0.00007122,
0.00008504,
0.00007450,
0.00008010,
0.00008936,
0.00008826,
0.00008831,
0.00009721,
0.00009019,
0.00008741,
0.00008467,
0.00007373,
0.00006748,
0.00006984,
0.00005307,
0.00004844,
0.00005417,
0.00004482,
0.00004483,
0.00003814,
0.00003524,
0.00002726,
0.00002455,
0.00002566,
0.00002043,
0.00001759,
0.00001473,
0.00001512,
0.00001132,
0.00000862,
0.00001262,
0.00000857,
0.00000918,
0.00000689,
0.00000671,
0.00000434,
0.00000648,
0.00000513,
0.00000748,
0.00000691,
0.00000789,
0.00000754,
0.00000715,
0.00000806,
0.00000701,
0.00000449,
0.00000640,
0.00000595,
0.00000877,
0.00000724,
0.00000837,
0.00000733,
0.00000815,
0.00000621,
0.00000755,
0.00000444,
0.00000580,
0.00000524,
0.00000343,
0.00000356,
0.00000340,
0.00000361,
0.00000245,
0.00000146,
0.00000232,
0.00000165,
0.00000168,
0.00000491,
0.00000168,
0.00000411,
0.00000122,
0.00000367,
0.00000244,
0.00000179,
0.00000395,
0.00000177,
0.00000108,
0.00000330,
0.00000166,
0.00000258,
0.00000204,
0.00000054,
0.00000259,
0.00000133,
0.00000205,
0.00000101,
0.00000210,
0.00000084,
0.00000090,
0.00000176,
0.00000079,
0.00000165,
0.00000071,
0.00000173,
0.00000070,
0.00000043,
0.00000056,
0.00000159
        };
      //B3 nir band
      const float b3[601] =
        {
0.00000184,
0.00000202,
0.00000371,
0.00000470,
0.00000487,
0.00000512,
0.00000521,
0.00000490,
0.00000308,
0.00000160,
0.00000126,
0.00000131,
0.00000119,
0.00000105,
0.00000082,
0.00000078,
0.00000076,
0.00000059,
0.00000058,
0.00000052,
0.00000064,
0.00000070,
0.00000068,
0.00000068,
0.00000079,
0.00000093,
0.00000124,
0.00000092,
0.00000124,
0.00000117,
0.00000125,
0.00000129,
0.00000121,
0.00000153,
0.00000149,
0.00000148,
0.00000139,
0.00000166,
0.00000211,
0.00000193,
0.00000241,
0.00000211,
0.00000228,
0.00000294,
0.00000406,
0.00000507,
0.00001753,
0.00062612,
0.00099041,
0.00038281,
0.00084800,
0.00090343,
0.00098820,
0.00100027,
0.00030612,
0.00003473,
0.00002650,
0.00002396,
0.00002429,
0.00002735,
0.00003712,
0.00005612,
0.00007768,
0.00008914,
0.00009611,
0.00010759,
0.00011246,
0.00011011,
0.00009290,
0.00006854,
0.00005014,
0.00003890,
0.00003142,
0.00002673,
0.00002266,
0.00002061,
0.00002008,
0.00002029,
0.00002207,
0.00002564,
0.00003063,
0.00003885,
0.00004812,
0.00005996,
0.00007077,
0.00007965,
0.00008609,
0.00008841,
0.00008691,
0.00008090,
0.00007026,
0.00005867,
0.00004706,
0.00003684,
0.00002888,
0.00002252,
0.00001864,
0.00001631,
0.00001512,
0.00001579,
0.00001740,
0.00002076,
0.00002500,
0.00002970,
0.00003324,
0.00003568,
0.00003719,
0.00003637,
0.00003522,
0.00003204,
0.00002806,
0.00002554,
0.00002401,
0.00002509,
0.00002676,
0.00003097,
0.00003703,
0.00004613,
0.00006025,
0.00007876,
0.00010325,
0.00012473,
0.00014349,
0.00015902,
0.00017128,
0.00017677,
0.00017814,
0.00017111,
0.00016498,
0.00016271,
0.00016590,
0.00017244,
0.00018189,
0.00018854,
0.00019224,
0.00019405,
0.00019354,
0.00018898,
0.00018087,
0.00017218,
0.00016359,
0.00015899,
0.00015586,
0.00015606,
0.00015819,
0.00016068,
0.00016530,
0.00016816,
0.00017049,
0.00017164,
0.00017169,
0.00017074,
0.00017054,
0.00016913,
0.00016744,
0.00016766,
0.00016951,
0.00017426,
0.00018167,
0.00019232,
0.00020428,
0.00021790,
0.00023244,
0.00024402,
0.00025416,
0.00026158,
0.00026375,
0.00026297,
0.00026655,
0.00026988,
0.00027504,
0.00028070,
0.00028807,
0.00030046,
0.00031707,
0.00033920,
0.00036610,
0.00039570,
0.00043575,
0.00048703,
0.00053825,
0.00057909,
0.00060314,
0.00061674,
0.00061491,
0.00059039,
0.00054258,
0.00047833,
0.00039787,
0.00031987,
0.00025214,
0.00019590,
0.00015376,
0.00012305,
0.00010304,
0.00008730,
0.00007548,
0.00006686,
0.00006139,
0.00005448,
0.00005006,
0.00004759,
0.00004287,
0.00003904,
0.00003667,
0.00003331,
0.00003138,
0.00002877,
0.00002657,
0.00002417,
0.00002328,
0.00002129,
0.00002036,
0.00001896,
0.00001779,
0.00001738,
0.00001579,
0.00001598,
0.00001498,
0.00001430,
0.00001381,
0.00001310,
0.00001315,
0.00001249,
0.00001236,
0.00001250,
0.00001178,
0.00001194,
0.00001165,
0.00001136,
0.00001177,
0.00001162,
0.00001149,
0.00001162,
0.00001162,
0.00001183,
0.00001163,
0.00001211,
0.00001182,
0.00001142,
0.00001146,
0.00001129,
0.00001064,
0.00001038,
0.00001123,
0.00001061,
0.00001039,
0.00001007,
0.00001068,
0.00001053,
0.00001131,
0.00001150,
0.00001210,
0.00001199,
0.00001228,
0.00001244,
0.00001267,
0.00001226,
0.00001236,
0.00001242,
0.00001236,
0.00001227,
0.00001222,
0.00001218,
0.00001214,
0.00001206,
0.00001202,
0.00001242,
0.00001231,
0.00001283,
0.00001325,
0.00001395,
0.00001410,
0.00001399,
0.00001534,
0.00001580,
0.00001690,
0.00001748,
0.00001885,
0.00001898,
0.00001988,
0.00002137,
0.00002320,
0.00002504,
0.00002641,
0.00002837,
0.00003079,
0.00003251,
0.00003533,
0.00003740,
0.00004081,
0.00004361,
0.00004639,
0.00004986,
0.00005346,
0.00005805,
0.00006226,
0.00006694,
0.00007188,
0.00007800,
0.00008485,
0.00009055,
0.00010008,
0.00010713,
0.00011933,
0.00012975,
0.00014324,
0.00015785,
0.00017539,
0.00019507,
0.00021689,
0.00024318,
0.00027135,
0.00030518,
0.00034281,
0.00039712,
0.00044651,
0.00050188,
0.00056707,
0.00063984,
0.00072378,
0.00081879,
0.00092782,
0.00105511,
0.00120160,
0.00137088,
0.00156792,
0.00180276,
0.00207920,
0.00240799,
0.00280298,
0.00327494,
0.00384821,
0.00454832,
0.00540493,
0.00645469,
0.00774941,
0.00936399,
0.01137832,
0.01388118,
0.01700597,
0.02092752,
0.02586928,
0.03206911,
0.03985523,
0.04956652,
0.06173849,
0.07680296,
0.09550239,
0.11837088,
0.14615821,
0.19110224,
0.23178705,
0.27801636,
0.33031673,
0.38771033,
0.44914649,
0.51370254,
0.57939644,
0.64484781,
0.70835304,
0.76788190,
0.82252523,
0.87063314,
0.91086251,
0.94360264,
0.96734308,
0.98447500,
0.99402844,
0.99904759,
1.00000000,
0.99829749,
0.99534944,
0.99113813,
0.98712837,
0.98344438,
0.97955733,
0.97683333,
0.97381307,
0.97171326,
0.96937935,
0.96682308,
0.96494693,
0.96155704,
0.95914861,
0.95524656,
0.95141098,
0.94725240,
0.94238136,
0.93805030,
0.93299243,
0.92787669,
0.92261726,
0.91765026,
0.91344782,
0.90866986,
0.90481152,
0.90031693,
0.89720732,
0.89342505,
0.89040645,
0.88745245,
0.88446834,
0.88140255,
0.87825555,
0.87487903,
0.87077850,
0.86665463,
0.86193864,
0.85704385,
0.85203260,
0.84608531,
0.84067874,
0.83464870,
0.82847182,
0.82237664,
0.81646034,
0.81067178,
0.80547624,
0.79996761,
0.79538371,
0.79048464,
0.78593335,
0.78246221,
0.77854086,
0.77505353,
0.77183963,
0.76834628,
0.76559075,
0.76238953,
0.75915219,
0.75631414,
0.75288758,
0.74984835,
0.74648701,
0.74311251,
0.73936992,
0.73559705,
0.73163277,
0.72765295,
0.72334537,
0.71892768,
0.71424776,
0.70932732,
0.70417024,
0.69865547,
0.69314542,
0.68706957,
0.68120878,
0.67553032,
0.66989091,
0.66440763,
0.65922988,
0.65415969,
0.64965635,
0.64537477,
0.64153052,
0.63820324,
0.63506916,
0.63264959,
0.62992135,
0.62726732,
0.62445640,
0.62156999,
0.61847181,
0.61501909,
0.61133939,
0.60744762,
0.60314205,
0.59883705,
0.59425662,
0.58928886,
0.58381508,
0.57719033,
0.56857851,
0.55699311,
0.54163099,
0.52202898,
0.49847084,
0.47191641,
0.44270534,
0.41155122,
0.37843868,
0.34304355,
0.30590964,
0.26730226,
0.22805403,
0.18974714,
0.15369596,
0.12145605,
0.09413162,
0.07189929,
0.05457376,
0.04144546,
0.03158417,
0.02425525,
0.01879617,
0.01470317,
0.01163795,
0.00930858,
0.00744330,
0.00607348,
0.00501379,
0.00413379,
0.00347084,
0.00293573,
0.00250919,
0.00215786,
0.00186591,
0.00163284,
0.00142997,
0.00127599,
0.00112918,
0.00102171,
0.00091069,
0.00083541,
0.00075489,
0.00069931,
0.00063879,
0.00059080,
0.00055023,
0.00050660,
0.00047777,
0.00044789,
0.00041317,
0.00039493,
0.00035947,
0.00033744,
0.00031989,
0.00028719,
0.00027707,
0.00025430,
0.00023132,
0.00022543,
0.00019537,
0.00018380,
0.00016713,
0.00014542,
0.00013876,
0.00011616,
0.00011019,
0.00009086,
0.00009183,
0.00006774,
0.00006181,
0.00006360,
0.00004633,
0.00005224,
0.00003735,
0.00003902,
0.00002393,
0.00003180,
0.00001716,
0.00001762,
0.00002033,
0.00001262,
0.00001259,
0.00001118,
0.00001421,
0.00000830,
0.00001294,
0.00000699,
0.00000782,
0.00000718,
0.00000642,
0.00000548,
0.00000427,
0.00000417,
0.00000440,
0.00000377,
0.00000253,
0.00000456,
0.00000202,
0.00000373,
0.00000325,
0.00000341,
0.00000232,
0.00000301,
0.00000117,
0.00000319,
0.00000169,
0.00000286,
0.00000184,
0.00000260,
0.00000212,
0.00000293,
0.00000175,
0.00000229,
0.00000126,
0.00000232,
0.00000099,
0.00000250,
0.00000135,
0.00000090,
0.00000289,
0.00000070,
0.00000210,
0.00000118,
0.00000072,
0.00000182,
0.00000111
        };
      //Add multispectral bands to the temporary list
      const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
      const std::vector<float> vb1 (b1, b1 + sizeof(b1) / sizeof(float) );
      const std::vector<float> vb2 (b2, b2 + sizeof(b2) / sizeof(float) );
      const std::vector<float> vb3 (b3, b3 + sizeof(b3) / sizeof(float) );
      //For Spot6 MS image the order of band is: B2 B1 B0 B3
      //(BandIndexToWavelength method could be used here)
      tmpSpectralBandList.push_back(vb2);
      tmpSpectralBandList.push_back(vb1);
      tmpSpectralBandList.push_back(vb0);
      tmpSpectralBandList.push_back(vb3);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Spot6 Sensor ID");
      }
    }
  else
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  unsigned int j = 0;
  for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
    {
    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.4);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.0);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.001);
    ++j;
    }
  return wavelengthSpectralBand;
}

} // end namespace otb
