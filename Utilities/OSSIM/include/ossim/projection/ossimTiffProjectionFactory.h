//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// class ossimProjectionFactory maintains a static list of projection
// makers.  If a user wants to create a projection they can pass in
// a projection name and a datum if desired.  Currently the default
// datum will be WGS84.
//*******************************************************************
//  $Id: ossimTiffProjectionFactory.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimTiffProjectionFactory_HEADER
#define ossimTiffProjectionFactory_HEADER

#include <ossim/projection/ossimProjectionFactoryBase.h>

class ossimProjection;
class ossimString;

class ossimTiffProjectionFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimTiffProjectionFactory* instance();

   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   /*!
    * METHOD: create()
    * Attempts to create an instance of the projection specified by name.
    * Returns successfully constructed projection or NULL.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /*!
    * METHOD: getList()
    * Returns list of all projections represented by this factory:
    */
   virtual std::list<ossimString> getList()const;

protected:
   ossimTiffProjectionFactory() {}
   static ossimTiffProjectionFactory*  theInstance;

   bool isTiff(const ossimFilename& filename)const;
};

#endif
