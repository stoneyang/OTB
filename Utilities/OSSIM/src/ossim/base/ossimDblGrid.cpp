//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR:  Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION:
//   Contains implementation of class ossimDblGrid. This class maintains
//   a regular grid of floating point (double) values. Access methods to the
//   grid include interpolation between grid nodes. Capability is included
//   to address the grid in an arbitrary, floating-point x, y coordinate
//   system. 
//
//*****************************************************************************
//  $Id: ossimDblGrid.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/base/ossimDblGrid.h>
#include <limits.h>
#include <iostream>
using namespace std;

#include <ossim/base/ossimDrect.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceExec  ("ossimDblGrid:exec");
static ossimTrace traceDebug ("ossimDblGrid:debug");

const double ossimDblGrid::DEFAULT_NULL_VALUE =  OSSIM_DBL_NAN;
static const ossimString MAGIC_NUMBER ("OSSIM_DBL_GRID");
static const int   MAX_LENGTH_DESCR = 80;

/*!****************************************************************************
 *  DEFAULT CONSTRUCTOR: ossimDblGrid
 *  
 *****************************************************************************/
ossimDblGrid::ossimDblGrid()
   :
      theGridData       (0),
      theSize           (0, 0),
      theOrigin         (0.0, 0.0),
      theSpacing        (0.0, 0.0),
      theMinValue       (OSSIM_INFINITY),
      theMaxValue       (-OSSIM_INFINITY),
      theNullValue      (DEFAULT_NULL_VALUE),
      theMeanIsComputed (false)
{ }


/*!****************************************************************************
 *  COPY CONSTRUCTOR: ossimDblGrid
 *  
 *****************************************************************************/
ossimDblGrid::ossimDblGrid(const ossimDblGrid&  source)
   :
      theGridData(NULL),
      theSize           (source.theSize),
      theOrigin         (source.theOrigin),
      theSpacing        (source.theSpacing),
      theMinValue       (source.theMinValue),
      theMaxValue       (source.theMaxValue),
      theNullValue      (source.theNullValue),
      theMeanValue      (source.theMeanValue),
      theDeviation      (source.theDeviation),
      theMeanIsComputed (source.theMeanIsComputed)
{
   static const char MODULE[] = "ossimDblGrid Constructor";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   *this = source;
   /*!
    * Allocate mem for the grid, and initialize:
    */
//    int buflen = theSize.x * theSize.y;
//    theGridData = new double [buflen];

//    for (int i=0; i<buflen; i++)
//       theGridData[i] = source.theGridData[i];
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
}

/*!****************************************************************************
 *  CONSTRUCTOR: ossimDblGrid
 *  
 *****************************************************************************/
ossimDblGrid::ossimDblGrid(const ossimIpt&  size, 
                           const ossimDpt&  origin,
                           const ossimDpt&  spacing,
                           double           null_value)
   :
      theGridData (0)
{
   static const char MODULE[] = "ossimDblGrid Constructor";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   initialize(size, origin, spacing, null_value);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
}

/*!****************************************************************************
 *  CONSTRUCTOR: ossimDblGrid
 *  
 *****************************************************************************/
ossimDblGrid::ossimDblGrid(const ossimDrect&  rect, 
                           const ossimDpt&    spacing,
                           double             null_value)
   :
      theGridData (0)
{
   static const char MODULE[] = "ossimDblGrid Constructor";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   ossimIpt size ((int) (rect.width()/spacing.x)  + 1,
                  (int) (rect.height()/spacing.y) + 1);
   
   initialize(size, rect.ul(), spacing, null_value);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_WARN) << MODULE << " returning...\n";
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimDblGrid()
//  
//*****************************************************************************
ossimDblGrid::~ossimDblGrid()
{
   if(theGridData)
   {
      delete [] theGridData;
      theGridData = NULL;
   }
}

//*****************************************************************************
//  METHOD: ossimDblGrid::initialize()
//  
//  Permits initialization after construction
//  
//*****************************************************************************
void ossimDblGrid::initialize(const ossimIpt&  size, 
                              const ossimDpt&  origin,
                              const ossimDpt&  spacing,
                              double           null_value)
{
   static const char MODULE[] = "ossimDblGrid::initialize()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_WARN) << MODULE << " entering...\n";

   //***
   // Delete any existing grid:
   //***
   if (theGridData)
    {
       delete [] theGridData;
       theGridData = 0;
    }
   
   //***
   // Initialize data members:
   //***
   theSize           = size;
   theOrigin         = origin;
   theSpacing        = spacing;
   theMinValue       = OSSIM_INFINITY;
   theMaxValue       =  -OSSIM_INFINITY;
   theNullValue      = null_value;
   theMeanIsComputed = false;

   /*!
    * Allocate mem for the grid, and initialize:
    */
   int buflen = theSize.x * theSize.y;
   if(buflen > 0)
     {
       theGridData = new double [buflen];

       for (int i=0; i<buflen; i++)
	 theGridData[i] = theNullValue;
     }   
   if (traceExec())  ossimNotify(ossimNotifyLevel_WARN) << MODULE << " returning...\n";

   return;
}

//*****************************************************************************
//  METHOD: ossimDblGrid::initialize()
//  
//  Permits initialization after construction
//  
//*****************************************************************************
void ossimDblGrid::initialize(const ossimDrect&  uv_rect, 
                              const ossimDpt&    spacing,
                              double null_value)
{
   static const char MODULE[] = "ossimDblGrid::initialize()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   ossimIpt size ((int) (uv_rect.width()/spacing.x)  + 1,
                  (int) (uv_rect.height()/spacing.y) + 1);
   
   initialize(size, uv_rect.ul(), spacing, null_value);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return;
}

void ossimDblGrid::deallocate()
{
  if(theGridData)
    {
      delete [] theGridData;
      theGridData = 0;
    }
  theSize = ossimIpt(0,0); 
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::setNode(x, y) NON-CONST
 *
 *  This method is used to assign the grid data values.
 *  
 *****************************************************************************/
void ossimDblGrid::setNode (int x, int y,
                            const double&   value) 
{
  if(!theGridData) return;

   if ((x>=0)&&(x<theSize.x)&&(y>=0)&&(y<theSize.y))
   {
      theGridData[index(x, y)] = value;

      if (value != theNullValue)
      {
         if (value < theMinValue)
            theMinValue = value;
         if (value > theMaxValue)
            theMaxValue = value;
      }
      
      theMeanIsComputed = false;
   }
   
   return;
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::setNearestNode(uv)
 *
 * Sets the node nearest the U,V point specified to the value given. This is
 * different from setNode() in that the UV coordinate system is used to
 * address a node instead of an XY grid point.
 *  
 *****************************************************************************/
void ossimDblGrid::setNearestNode (const ossimDpt& uv_point,
                                   const double&   value) 
{
  if(!theGridData) return;
   //***
   // Establish the grid indexes:
   //***
   int xi = (int) irint((uv_point.u - theOrigin.u)/theSpacing.x);
   int yi = (int) irint((uv_point.v - theOrigin.v)/theSpacing.y);

   if (xi < 0)
      xi = 0;
   if (xi >= theSize.x)
      xi = theSize.x - 1;
   if (yi < 0)
      yi = 0;
   if (yi >= theSize.y)
      yi = theSize.y - 1;

   setNode (xi, yi, value);
   
   return;
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::getNode(x, y) CONST
 *
 *  This method is used to return the grid data values.
 *  
 *****************************************************************************/
double ossimDblGrid::getNode (int x, int y) const
{
  if(!theGridData) return theNullValue;
   if ((x>=0)&&(x<theSize.x)&&(y>=0)&&(y<theSize.y))
      return theGridData[index(x, y)];

   return theNullValue;
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::operator(double, double)
 *
 *  This method interpolates between grid points given a fractional location
 *  in UV (external) world space.
 *  
 *****************************************************************************/
double ossimDblGrid::operator() (const double& u,
                                 const double& v) const
{
  if(!theGridData) return theNullValue;

   if (isInside(u, v))
   {
      /*!
       * Establish the grid indexes:
       */
      double xi = (u - theOrigin.u)/theSpacing.x;
      double yi = (v - theOrigin.v)/theSpacing.y;

      
      int x0 = (int) xi;
      int y0 = (int) yi;

      x0 = (x0<theSize.x)?x0:(theSize.x-1);
      y0 = (y0<theSize.y)?y0:(theSize.y-1);
      xi = (xi<theSize.x)?xi:(theSize.x-1);
      yi = (yi<theSize.y)?yi:(theSize.y-1);
      /*!
       * Perform bilinear interpolation:
       */
      double wx1 = xi - x0;
      double wy1 = yi - y0;

      
       double wx0 = 1.0 - wx1;
       double wy0 = 1.0 - wy1;

       int index00;
       int index10;
       int index11;
       int index01;

       
       index00  = theSize.x*y0 + x0;
       if(x0 < (theSize.x-1))
       {
          index10  = index00 + 1;
       }
       else
       {
          index10  = index00;
       }
       if(y0 < (theSize.y-1))
       {
          index01  = index00 + theSize.x;
       }
       else
       {
          index01  = index00;
       }
       if(y0 < (theSize.y-1))
       {
          index01  = index00 + theSize.x;
       }
       else
       {
          index01  = index00;
       }
       
       if(x0 < (theSize.x-1))
       {
          index11  = index01 + 1;
       }
       else
       {
          index11  = index01;
       }


      double p00   = (double) theGridData[index00];
      double p01   = (double) theGridData[index01];
      double p10   = (double) theGridData[index10];
      double p11   = (double) theGridData[index11];
      
      double w00   = wx0*wy0;
      double w01   = wx0*wy1;
      double w10   = wx1*wy0;
      double w11   = wx1*wy1;
      
      return (p00*w00 + p01*w01 + p10*w10 + p11*w11) /
         (w00 + w01 + w10 + w11);
   }

   // else
   return theNullValue;
}

//*****************************************************************************
//  OPERATOR: =
//  
//*****************************************************************************
const ossimDblGrid& ossimDblGrid::operator = (const ossimDblGrid& source)
{
   if(&source == this) return *this;
   
   if (theGridData)
     {
       delete [] theGridData;
       theGridData = 0;
     }

   //***
   // Assign data members:
   //***
   theSize           = source.theSize;
   theOrigin         = source.theOrigin;
   theSpacing        = source.theSpacing;
   theMinValue       = source.theMinValue;
   theMaxValue       = source.theMaxValue;
   theNullValue      = source.theNullValue;
   theMeanValue      = source.theMeanValue;
   theDeviation      = source.theDeviation;
   theMeanIsComputed = source.theMeanIsComputed;

   //***
   // Allocate mem for the grid, and initialize:
   //***
   int buflen = theSize.x * theSize.y;
   if(buflen>0)
     {
       theGridData = new double [buflen];
       
       for (int i=0; i<buflen; i++)
	 {
	   theGridData[i] = source.theGridData[i];
	 }
     }
   return *this;
}


/*!****************************************************************************
 * METHOD: ossimDblGrid::meanValue()
 *  
 *****************************************************************************/
double  ossimDblGrid::meanValue()
{
   static const char MODULE[] = "ossimDblGrid::meanValue()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";
   
   if (!theMeanIsComputed)
     {
       computeMean();
     }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return theMeanValue;
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::meanStdDev()
 *  
 *****************************************************************************/
double  ossimDblGrid::meanStdDev()
{
   static const char MODULE[] = "ossimDblGrid::meanStdDev()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";
   
   if (!theMeanIsComputed)
      computeMean();

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";;
   return theDeviation;
}

/*!****************************************************************************
 * METHOD: ossimDblGrid::computeMean()
 *  
 *****************************************************************************/
void ossimDblGrid::computeMean()
{
   static const char MODULE[] = "ossimDblGrid::meanStdDev()";
  if(!theGridData) return;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "entering...\n";

   if (!theMeanIsComputed)
   {
      double accum = 0.0;
      double num_samples = 0.0;

      /*!
       * Loop to compute mean:
       */
      for (int i=0; i<(theSize.x*theSize.y); i++)
      {
         if (theGridData[i] != theNullValue)
         {
            accum += theGridData[i];
            num_samples += 1.0;
         }
      }
      theMeanValue = accum/num_samples;

      /*!
       * Loop again to compute deviation:
       */
      accum = 0.0;
      double diff;
      for (int i=0; i<(theSize.x*theSize.y); i++)
      {
         if (theGridData[i] != theNullValue)
         {
            diff = theMeanValue - theGridData[i];
            accum += diff*diff;
         }
      }
      theDeviation = sqrt(accum/num_samples);

      theMeanIsComputed = true;
   }
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return;
}

/*!****************************************************************************
 * INLINE METHOD: ossimDblGrid::isInside(const ossimDpt& pt) const
 *****************************************************************************/
bool ossimDblGrid::isInside(const double& u,
                                   const double& v) const
{
   double xi = (u - theOrigin.u)/theSpacing.x;
   double yi = (v - theOrigin.v)/theSpacing.y;
//   return ((xi >= 0.0) && (xi <= ((double)theSize.x - 1.0)) &&
//           (yi >= 0.0) && (yi <= ((double)theSize.y - 1.0)));
   return ((xi >= 0.0) && (xi < ((double)theSize.x)) &&
           (yi >= 0.0) && (yi < ((double)theSize.y)));
}
                    

//*****************************************************************************
//  METHOD: ossimDblGrid::save()
//  
//  Saves the grid to the stream in compact ASCII format (not necessarily
//  human readable).
//  
//*****************************************************************************
bool ossimDblGrid::save(ostream& os, const char* descr) const
{
   static const char MODULE[] = "ossimDblGrid::save()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   //***
   // Preserve the stream's settings:
   //***
   ios::fmtflags new_options = ios::scientific|ios::dec;
   //ios::streamsize new_precision = 12;
   int new_precision = 12;
   
   ios::fmtflags old_options = os.flags(new_options);
   int old_precision = os.precision(new_precision);

   //***
   // Verify the description string is not too long:
   //***
   char descr_buf[81];
   strncpy(descr_buf, descr, 80);
   descr_buf[80] = '\0';
   
   //***
   // write magic number tag and the grid size X, Y, num params:
   //***
   os << MAGIC_NUMBER << " " << descr_buf << "\n"
      << theSize.x << "  "
      << theSize.y << "  "
      << theOrigin.u << "  "
      << theOrigin.v << "  "
      << theSpacing.u << "  "
      << theSpacing.v << "  "
      << theNullValue << "\n";

   if(theGridData)
     {
       //***
       // Loop to write grid points:
       //***
       int max_index = theSize.x*theSize.y;
       for (int i=0; i<max_index; i++)
	 os << theGridData[i] << "  ";
     }
   os << "\n";
   
   //***
   // Restore the stream's state:
   //***
   os.flags(old_options);
   os.precision(old_precision);
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return true;
}

//*****************************************************************************
//  METHOD: ossimDblGrid::load()
//  
//  Loads the grid from the stream in compact ASCII format (not necessarily
//  human readable).
//  
//*****************************************************************************
bool ossimDblGrid::load(istream& is)
{
   static const char MODULE[] = "ossimDblGrid::load()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";

   char strbuf[128];
   
   //***
   // Read magic number tag to insure it is an ossimDblGrid record:
   //***
   is >> strbuf;
   if (strncmp(strbuf, MAGIC_NUMBER.c_str(), MAGIC_NUMBER.length()))
   {
      ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Error reading OSSIM_DBL_GRID magic number from stream. "
                                          << "Aborting...\n";
      return false;
   }
   is.getline(strbuf, 128, '\n');
   theSize           = ossimDpt(0,0);
   theOrigin         = ossimDpt(0,0);
   theSpacing        = ossimDpt(0,0);
   theMinValue       = OSSIM_INFINITY;
   theMaxValue       =  -OSSIM_INFINITY;
   theNullValue      = theNullValue;
   theMeanIsComputed = false;

   
   //***
   // Read the grid size, origin, and spacing:
   //***
   ossimIpt size;
   ossimDpt origin, spacing;
   double null_value;
   is >> size.x 
      >> size.y 
      >> origin.u 
      >> origin.v 
      >> spacing.u
      >> spacing.v
      >> null_value;
   
   initialize(size, origin, spacing, null_value);
   
   //***
   // Loop to read grid points:
   //***
   int max_index = theSize.x*theSize.y;
   for (int i=0; i<max_index; i++)
     {
       is >> theGridData[i];
     }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   
   return true;
}

//*****************************************************************************
//  PRIVATE FUNCTION: isBlocked()
//  
//  Used by interpolateNullValuedNodes. Returns true if the direction indicated
//  by the vector (x, y) has been blocked from further sampling. This occurs if
//  a sample has already been found in that general direction. The directions
//  are discrete (16) evenly distributed about a "compass rose." Each index
//  corresponds to one of the directions as illustrated here:
//
//                        15 0 1
//                     14    |    2
//                    13     |     3
//                   12------+------4
//                    11     |     5
//                     10    |    6
//                         9 8 7
//
//*****************************************************************************
bool isBlocked(bool* blocked, int x, int y)
{
   if (y == 0)
   {
      if (x > 0) return blocked[4];
      return blocked[12];
   }
   
   double r = x/y;
   int c = 0;
   int i;
   if (x < 0.0) c = 8;

   //***
   // Test the tangent value instead of computing angle:
   //***
   if      (r >  5.02734) i = 12 + c;
   else if (r >  1.49660) i = 13 + c;
   else if (r >  0.66818) i = 14 + c;
   else if (r >  0.19891) i = 15 + c;
   else if (r > -0.19891) i =  0 + c;
   else if (r > -0.66818) i =  1 + c;
   else if (r > -1.49660) i =  2 + c;
   else if (r > -5.02734) i =  3 + c;
   else                   i =  4 + c;

   if (i > 15) i -= 16;  // modulo 16
   
   return blocked[i];
}

//*****************************************************************************
//  PRIVATE FUNCTION: blockDirection()
//  
//  Used by interpolateNullValuedNodes. Blocks the resampler from exploring
//  further in a general direction, specified by thevector (x, y). The blocking
//  is requested when a sample is found. This prevents a sample that is shadowed
//  by a closer sample from having influence.
//
//  See method isBlocked() above for a description of the compass rose indexing.
//  
//*****************************************************************************
void blockDirection(bool* blocked, int x, int y)
{
   if (y == 0)
   {
      if (x > 0) blocked[4] = true;
      else       blocked[12] = true;
      return;
   }

   double r = x/y;
   int c = 0;
   int i;
   if (x < 0.0) c = 8;

   //***
   // Test the tangent value instead of computing angle:
   //***
   if      (r >  5.02734) i = 12 + c;
   else if (r >  1.49660) i = 13 + c;
   else if (r >  0.66818) i = 14 + c;
   else if (r >  0.19891) i = 15 + c;
   else if (r > -0.19891) i =  0 + c;
   else if (r > -0.66818) i =  1 + c;
   else if (r > -1.49660) i =  2 + c;
   else if (r > -5.02734) i =  3 + c;
   else                   i =  4 + c;

   if (i > 15) i -= 16;  // modulo 16
   blocked[i] = true;
   
   return;
}
      
//*****************************************************************************
//   METHOD: ossimDblGrid::interpolateNullValuedNodes(decay_rate)
//
//   This method performs a resampling of the defined grid nodes in order to
//   compute interpolated values for those uninitialized nodes. This is
//   necessary when only a subset of nodes are available for initializing the
//   grid.
//
//   The decay rate is a geometric (1/r) factor applied to the weights to
//   amplify the rate at which a neighbor's influence diminishes. For a
//   decay_rate = 1, the influence of a sample diminishes linearly with the
//   distance.
//  
//*****************************************************************************
void ossimDblGrid::interpolateNullValuedNodes(const double& decay_rate)
{
   static const char MODULE[] = "ossimDblGrid::interpolateNullValuedNodes()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entering...\n";
   if(!theGridData) return;

   //***
   // Allocate buffer to store resampled nodes:
   //***
   int      buf_size          = theSize.x*theSize.y;
   double*  resampled_grid    = new double [buf_size];
   double   min_weight_needed = 4.0/decay_rate;

   int      start_x, start_y, end_x, end_y;
   int      diameter;
   double   sum_weights;
   double   accumulator;
   double   weight;
   double   sample;
   double   node_value;
   bool     sample_found;
   int      node_idx;
   double   adj_delta;
   int      dx, dy;
   bool     blocked[16];

   //***
   // Loop over the entire grid to resample all NULL nodes:
   //***
   for (int y=0; y<theSize.y; y++)
   {
      for (int x=0; x<theSize.x; x++)
      {
         //***
         // Only resample those nodes that contain NULL:
         //***
         node_idx = index(x, y);
         node_value = theGridData[node_idx];
         if (node_value != theNullValue)
         {
            //***
            // This node had a value. Simply copy it into the resample_grid:
            //***
            resampled_grid[node_idx] = node_value;
         }

         else
         {
            //***
            // Resampling is necessary. Initialize quantities used:
            //***
            start_x = x;
            start_y = y;
            diameter = 0;
            sum_weights = 0;
            accumulator = 0;
            weight = 1.0;
            sample_found = true;

            for (int i=0; i<16; i++)
               blocked[i] = false;
            
            //***
            // Loop collecting contributions from non-null neighbors. Begin with
            // a small kernel size (diameter) and successively grow until a
            // sufficient number of contributors is found:
            //***
            while ((sum_weights < min_weight_needed) && sample_found)
            {
               diameter += 2;
               start_x -= 1;
               start_y -= 1;
               weight *= decay_rate;
               sample_found = false;

               //***
               // Loop over each pixel in kernel and sum in it's contribution:
               //***
               end_y = start_y + diameter;
               end_x = start_x + diameter;
               
               for (int yn=start_y; yn<=end_y; yn++)
               {
                  if ((yn == start_y) || (yn == end_y))
                  {
                     //***
                     // This is the top edge or bottom edge, need samples from
                     // each x along kernel edge:
                     //***
                     for (int xn=start_x; xn<=end_x; xn++)
                     {
                        sample_found = sample_found || isInside(xn, yn);
                        sample = getNode(xn, yn);
                        if (sample != theNullValue)
                        {
                           dx = x - xn; dy = y - yn;
                           if (!isBlocked(blocked, dx, dy))
                           {
                              adj_delta = weight*sqrt((double)(dx*dx + dy*dy));
                              accumulator += sample/adj_delta;
                              sum_weights += 1.0/adj_delta;
                              blockDirection(blocked, dx, dy);
                           }
                        }
                     }
                  }
                  else
                  {
                     //***
                     // For the left/right edge of the kernel, need to sample
                     // only the first (start_x) and last (end_x):
                     //***
                     sample_found = sample_found || isInside(start_x, yn);
                     sample = getNode(start_x, yn);
                     if (sample != theNullValue)
                     {
                        dx = x - start_x; dy = y - yn;
                        if (!isBlocked(blocked, dx, dy))
                        {
                           adj_delta = weight*sqrt((double)(dx*dx + dy*dy));
                           accumulator += sample/adj_delta;
                           sum_weights += 1.0/adj_delta;
                           blockDirection(blocked, dx, dy);
                        }
                     }

                     sample_found = sample_found || isInside(end_x, yn);
                     sample = getNode(end_x, yn);
                     if (sample != theNullValue)
                     {
                        dx = x - end_x; dy = y - yn;
                        if (!isBlocked(blocked, dx, dy))
                        {
                           adj_delta = weight*sqrt((double)(dx*dx + dy*dy));
                           accumulator += sample/adj_delta;
                           sum_weights += 1.0/adj_delta;
                           blockDirection(blocked, dx, dy);
                        }
                     }
                  } // end else
               } // end for loop over all rows in kernel
            } // end while loop inflating kernel size

            //***
            // Finished collecting sample contributions for this node. compute
            // convolution and save in buffer:
            //***
            if (sum_weights != 0)
               resampled_grid[node_idx] = accumulator/sum_weights;
            else
               resampled_grid[node_idx] = theNullValue;

         } // end else (if input node is NULL)
      } // end for loop over all columns in grid
   } // end for loop over all lines in grid

   //***
   // Now copy the resampled grid back into the original buffer:
   //***
   for (node_idx=0; node_idx<buf_size; node_idx++)
      theGridData[node_idx] = resampled_grid[node_idx];

   delete [] resampled_grid;
               
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return;
}

//*****************************************************************************
//  METHOD: ossimDblGrid::filter(size_x, size_y, kernel)
//  
//  Passes the grid data through a convolution filter given in the kernel array.
//  The grid must not contain any NULL nodes as these are not scanned for.
//  The kernel sizes should be odd numbers.
//  
//*****************************************************************************
void ossimDblGrid::filter(int size_x, int size_y, double* kernel)
{
   static const char MODULE[] = "ossimDblGrid::filter()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << "entering...\n";
   if(!theGridData) return;

   //***
   // Allocate buffer to store resampled nodes:
   //***
   int      buf_size          = theSize.x*theSize.y;
   double*  resampled_grid    = new double [buf_size];
   for (int i=0; i<buf_size; i++)
      resampled_grid[i] = 0.0;

   int      rx      = (size_x - 1)/2;     // kernel radii
   int      ry      = (size_y - 1)/2;     
   int      start_x = rx;                 // indexes to start sampling grid
   int      start_y = ry;
   int      end_x   = theSize.x - rx;     // indexes to end buffer sampling
   int      end_y   = theSize.y - ry;
   int      knl_ctr = ry*size_x + rx;     // offset to center of kernel buffer
   double   node_value;
   double   kernel_value;
   int      node_idx;
   
   //***
   // Loop over the entire grid to resample all NULL nodes:
   //***
   for (int y=start_y; y<end_y; y++)
   {
      for (int x=start_x; x<end_x; x++)
      {
         node_idx = index(x, y);
         
         //***
         // Fetch samples for each kernel element, apply gain, then accumulate
         // in output buffer:
         //***
         for (int ky=-ry; ky<=ry; ky++)
         {
            for (int kx=-rx; kx<=rx; kx++)
            {
               node_value = theGridData[index(x+kx, y+ky)];
               kernel_value = kernel[knl_ctr + ky*size_x + kx];
               resampled_grid[node_idx] += kernel_value*node_value;
            }
         }
      }
   }

   //***
   // The grid margin has unfiltered data due to the kernel radius. Set these
   // nodes equal to the nearest filtered nodes:
   //***
   for (int y=0; y<theSize.y; y++)
   {
      for (int x=0; x<theSize.x; x++)
      {
         //***
         // Top edge:
         //***
         if (y < start_y)
         {
            if (x < start_x)      // top-left corner
               resampled_grid[index(x, y)] = resampled_grid[index(rx, ry)];
            else if (x >= end_x)  // top-right corner
               resampled_grid[index(x, y)] = resampled_grid[index(end_x-1, ry)];
            else                  // top region
               resampled_grid[index(x, y)] = resampled_grid[index(x, ry)];
         }

         //***
         // Middle:
         //***
         else if (y < end_y)
         {
            if (x < start_x)      // left edge
               resampled_grid[index(x, y)] = resampled_grid[index(rx, y)];
            else if (x >= end_x)  // right edge
               resampled_grid[index(x, y)] = resampled_grid[index(end_x-1, y)];
         }

         //***
         // Bottom:
         //***
         else
         {
            if (x < start_x)      // bottom-left corner
               resampled_grid[index(x, y)] = resampled_grid[index(rx, end_y-1)];
            else if (x >= end_x)  // bottom-right corner
               resampled_grid[index(x, y)] = resampled_grid[index(end_x-1,
                                                                  end_y-1)];
            else                  // bottom region
               resampled_grid[index(x, y)] = resampled_grid[index(x, end_y-1)];
         }
      }
   }
   
   //***
   // Now copy the resampled grid back into the original buffer:
   //***
   for (node_idx=0; node_idx<buf_size; node_idx++)
      theGridData[node_idx] = resampled_grid[node_idx];

   delete [] resampled_grid;
               
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " returning...\n";
   return;
}


//*****************************************************************************
//  METHOD: ossimDblGrid::fill
//  
//  Fills the current grid with constant value provided.
//  
//*****************************************************************************
void ossimDblGrid::fill(double fill_value)
{
   if (!theGridData)
     {
       return;
     }
   
   int size = theSize.x * theSize.y;
   for (int i=0; i<size; i++)
      theGridData[i] = fill_value;
   
   return;
}


//*****************************************************************************
//  FRIEND OPERATOR: ostream& << (ostream&) 
//  
//*****************************************************************************
ostream& operator<<(ostream& os, const ossimDblGrid& grid) 
{
   os << "\nDump of ossimDblGrid at " << (void*) &grid
      << "\n  theSize: "           << grid.theSize
      << "\n  theOrigin: "         << grid.theOrigin
      << "\n  theSpacing: "        << grid.theSpacing
      << "\n  theMinValue: "       << grid.theMinValue
      << "\n  theMaxValue: "       << grid.theMaxValue
      << "\n  theNullValue: "      << grid.theNullValue
      << "\n  theMeanValue: "      << grid.theMeanValue
      << "\n  theDeviation: "      << grid.theDeviation
      << "\n  theMeanIsComputed: " << grid.theMeanIsComputed
      << "\n";

   if(grid.theGridData)
     {
       
       for (int y=0; y<grid.theSize.y; y++)
	 {
	   for (int x=0; x<grid.theSize.x; x++)
	     {
	       os << "\n  node(" << x << ", " << y << "): " << grid.getNode(x,y);
	     }
	 }
     }

   return os;
}
