//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimConvolutionSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimConvolutionSource_HEADER
#define ossimConvolutionSource_HEADER
#include <vector>
#include <iostream>
#include <ossim/matrix/newmat.h>
using namespace std;

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimTilePatch;
class ossimDiscreteConvolutionKernel;

class ossimConvolutionSource : public ossimImageSourceFilter
{
public:
   ossimConvolutionSource();
   ossimConvolutionSource(ossimImageSource* inputSource,
                          const NEWMAT::Matrix& convolutionMatix);
   ossimConvolutionSource(ossimImageSource* inputSource,
                          const vector<NEWMAT::Matrix>& convolutionList);
   virtual ~ossimConvolutionSource();
   
   virtual void setConvolution(const double* kernel, int nrows, int ncols, bool doWeightedAverage=false);
   virtual void setConvolution(const NEWMAT::Matrix& convolutionMatrix, bool doWeightedAverage=false);
   virtual void setConvolutionList(const vector<NEWMAT::Matrix>& convolutionList, bool doWeightedAverage=false);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
protected:

   void allocate();
   
   ossimRefPtr<ossimImageData> theTile;
   long                        theMaxKernelWidth;
   long                        theMaxKernelHeight;
   
   vector<ossimDiscreteConvolutionKernel*> theConvolutionKernelList;
   virtual void setKernelInformation();
   virtual void deleteConvolutionList();

   template<class T>
   void convolve(T dummyVariable,
                 ossimRefPtr<ossimImageData> inputTile,
                 ossimDiscreteConvolutionKernel* kernel);
   

TYPE_DATA
};

#endif /* #ifndef ossimConvolutionSource_HEADER */
