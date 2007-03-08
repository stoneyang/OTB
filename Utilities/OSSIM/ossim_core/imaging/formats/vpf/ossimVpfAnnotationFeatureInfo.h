#ifndef ossimVpfAnnotationFeatureInfo_HEADER
#define ossimVpfAnnotationFeatureInfo_HEADER
#include "base/data_types/color_space/ossimRgbVector.h"
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimGeoPolygon.h"
#include "imaging/annotation/ossimGeoAnnotationObject.h"
#include "vec/vpf/ossimVpfCoverage.h"
#include "base/common/ossimFontInformation.h"

class ossimGeoAnnotationObject;
class ossimVpfCoverage;
class ossimVpfFeatureClassSchema;
class ossimPolyLine;

class OSSIMDLLEXPORT ossimVpfAnnotationFeatureInfo : public ossimObject
{
public:
  enum ossimVpfAnnotationFeatureType
    {
      ossimVpfAnnotationFeatureType_UNKNOWN  = 0,
      ossimVpfAnnotationFeatureType_POINT    = 1,
      ossimVpfAnnotationFeatureType_LINE     = 2,
      ossimVpfAnnotationFeatureType_POLYGON  = 3,
      ossimVpfAnnotationFeatureType_TEXT     = 4,
    };

  ossimVpfAnnotationFeatureInfo(const ossimString& featureName   = "",
				const ossimRgbVector& penColor   = ossimRgbVector(255,255,255),
				const ossimRgbVector& brushColor = ossimRgbVector(255,255,255),
                                const ossimDpt& pointRadius=ossimDpt(1,1),
                                int   thickness=0,
				bool enabledFlag = true);
  ~ossimVpfAnnotationFeatureInfo();

  void setCoverage(const ossimVpfCoverage& coverage)
  {
    theCoverage = coverage;
  }

  const ossimVpfCoverage& getCoverage()const
  {
    return theCoverage;
  }
  ossimVpfCoverage& getCoverage()
  {
    return theCoverage;
  }

   void setColor(const ossimRgbVector& color)
      {
         setPenColor(color);
         setBrushColor(color);
      }
  void setPenColor(const ossimRgbVector& penColor)
  {
    thePenColor = penColor;
  }

  ossimRgbVector getPenColor()const
  {
    return thePenColor;
  }

  void setBrushColor(const ossimRgbVector& brushColor)
  {
    theBrushColor = brushColor;
  }

  ossimRgbVector getBrushColor()const
  {
    return theBrushColor;
  }

  void setFillEnabledFlag(bool flag)
  {
     theFillEnabledFlag = flag;
  }
  bool getFillEnabledFlag()const
  {
     return theFillEnabledFlag;
  }
  void setEnabledFlag(bool flag);
  bool getEnabledFlag()const
  {
    return theEnabledFlag;
  }

  void setThickness(int thickness)
  {
     theThickness = thickness;
  }

  int getThickness()const
  {
     return theThickness;
  }
  
  void setName(const ossimString& name)
  {
    theName = name;
  }
  const ossimString& getName()const
  {
    return theName;
  }
   
  bool isPoint()const
  {
     return (theFeatureType == ossimVpfAnnotationFeatureType_POINT);
  }
  bool isLine()const
  {
     return (theFeatureType == ossimVpfAnnotationFeatureType_LINE);
  }
  bool isPolygon()const
  {
     return (theFeatureType == ossimVpfAnnotationFeatureType_POLYGON);
  }
  bool isText()const
  {
     return (theFeatureType == ossimVpfAnnotationFeatureType_TEXT);
  }
  void getFontInformation(ossimFontInformation& fontInfo)const
  {
     fontInfo = theFontInformation;
  }
  void setFontInformation(const ossimFontInformation& fontInfo)
  {
     theFontInformation = fontInfo;
  }

  void setPointRadius(const ossimDpt& radius)
  {
    thePointRadius = radius;
  }
  ossimDpt getPointRadius()const
  {
    return thePointRadius;
  }
  void setDrawingFeaturesToAnnotation();
  ossimIrect getBoundingProjectedRect()const;
  void transform(ossimProjection* proj);
  void buildFeature();
  void deleteAllObjects();
  void drawAnnotations(ossimRgbImage* tile);

  bool saveState(ossimKeywordlist& kwl,
		 const char* prefix=0)const;
  bool loadState(const ossimKeywordlist& kwl,
		 const char* prefix=0);
  ossimString getFeatureTypeAsString()const;
  ossimVpfAnnotationFeatureType getFeatureTypeFromString(const ossimString& featureType)const;

private:
  void readAttributes(ossimPolyLine& polyLine, ossimVpfTable& table, int row);
  void readAttributes(ossimGeoPolygon& polygon, ossimVpfTable& table, int row);

protected:
  ossimString                   theName;
  ossimString                   theDescription;
  ossimRgbVector                thePenColor;
  ossimRgbVector                theBrushColor;
  ossimVpfCoverage              theCoverage;
  ossimDpt                      thePointRadius;
  int                          theThickness; 
  bool                         theFillEnabledFlag;
  bool                         theEnabledFlag;
  ossimVpfAnnotationFeatureType theFeatureType;
  ossimFontInformation          theFontInformation;
   
  std::vector<ossimGeoAnnotationObject*> theAnnotationArray;

  void buildTxtFeature(const ossimFilename& table,
                       const ossimString&   tableKey,
                       const ossimFilename& primitive,
                       const ossimString&   primitiveKey);
   
  void buildEdgFeature(const ossimFilename& table,
                       const ossimString&   tableKey,
		       const ossimFilename& primitive,
		       const ossimString&   primitiveKey);

  void buildPointFeature(const ossimString& primitiveName,
			 const ossimFilename& table,
			 const ossimString&   tableKey,
			 const ossimFilename& primitive,
			 const ossimString&   primitiveKey);
  void buildFaceFeature(const ossimFilename& table,
                        const ossimString&   tableKey,
                        const ossimFilename& primitive,
			const ossimString&   primitiveKey);
  

  void readEdge(ossimPolyLine& polyLine,
		int rowNumber,
		int colPosition,
		ossimVpfTable& edgeTable);
  int readTableCellAsInt (int rowNumber, // should replace calls to readRngId, readStartEdgeId with this
			  int colNumber,
			  ossimVpfTable& table);
  int readRngId(int rowNumber,
		int colNumber,
		ossimVpfTable& faceTable);
  int readStartEdgeId(int rowNumber,
		      int colNumber,
		      ossimVpfTable& rngTable);
  void readGeoPolygon(ossimGeoPolygon& polygon,
		      int faceId,
		      int startEdgeId,
		      ossimVpfTable& edgTable);

  ossimDpt* getXy(vpf_table_type table, row_type row, long pos, long* count);
  int getEdgeKeyId (vpf_table_type& table, row_type& row, int col);

TYPE_DATA
};

#endif
