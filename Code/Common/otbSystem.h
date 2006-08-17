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
#ifndef _otbSystem_h
#define _otbSystem_h

#include <string>
#include <vector>

#include "itkWin32Header.h"



#ifdef _WIN
      #define OTB_FILE_SEPARATOR '\\'
#else 
     #define OTB_FILE_SEPARATOR '/'     
#endif

namespace otb
{


/** \class System
 * \brief System operations.
 *
 * System operations, filename manipulations, etc.
 *
 */
class ITK_EXPORT System 
{
public:

  /** Standard class typedefs. */
  typedef System  Self;

  /** Return true if the parameter 'pszPath' specify a directory name) */
  static bool IsADirName(std::string pszPath);
  /** Return true if the parameter 'pszPath' specify a file name) */
  static bool IsAFileName(std::string pszPath);
  /** Get the extension of the file name */
  static std::string GetExtension( const std::string& filename );
  /** Get the root name */
  static std::string GetRootName( const std::string& filename );
  /** Get the Path of the file name */
  static std::string GetPathName( const std::string& filename );
  /** Get the short filename of the file name */
  static std::string GetShortFileName( const std::string& filename );

  /** Get list of file find in a directory */
  static std::vector<std::string> Readdir(std::string pszPath);
};

} // namespace otb


#endif
