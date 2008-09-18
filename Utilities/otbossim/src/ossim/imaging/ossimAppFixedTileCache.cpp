//******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This file contains the Application cache algorithm
//
//***********************************
// $Id: ossimAppFixedTileCache.cpp 12653 2008-04-10 15:32:46Z gpotts $
#include <algorithm>
#include <sstream>
#include <ossim/imaging/ossimAppFixedTileCache.h>
#include <ossim/imaging/ossimFixedTileCache.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <OpenThreads/ScopedLock>

ossimAppFixedTileCache* ossimAppFixedTileCache::theInstance = 0;
ossimAppFixedTileCache::ossimAppFixedCacheId ossimAppFixedTileCache::theUniqueAppIdCounter = 0;
const ossim_uint32 ossimAppFixedTileCache::DEFAULT_SIZE = 1024*1024*80;

static const ossimTrace traceDebug("ossimAppFixedTileCache:debug");
std::ostream& operator <<(std::ostream& out, const ossimAppFixedTileCache& rhs)
{
   std::map<ossimAppFixedTileCache::ossimAppFixedCacheId, ossimFixedTileCache*>::const_iterator iter = rhs.theAppCacheMap.begin();

   if(iter == rhs.theAppCacheMap.end())
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "***** APP CACHE EMPTY *****" << endl;
   }
   else
   {
      while(iter != rhs.theAppCacheMap.end())
      {
         out << "Cache id = "<< (*iter).first << " size = " << (*iter).second->getCacheSize() << endl;
         ++iter;
      }
   }

   return out;
}


ossimAppFixedTileCache::ossimAppFixedTileCache()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimAppFixedTileCache::ossimAppFixedTileCache() DEBUG: entered ..." << std::endl;
   }
   theInstance = this;
   theTileSize = ossimIpt(64, 64);
   theCurrentCacheSize = 0;

   // ossim::defaultTileSize(theTileSize);
   
   ossim_uint32 cacheSize = ossimString(ossimPreferences::instance()->findPreference("cache_size")).toUInt32()*(1024*1024);
   const char* tileSize = ossimPreferences::instance()->findPreference("tile_size");
   if(tileSize)
   {
      std::stringstream in(tileSize);
      in >> theTileSize.x >> theTileSize.y;
      if(theTileSize.x < 1) theTileSize.x = 64;
      if(theTileSize.y < 1) theTileSize.y = 64;
   }
   if(cacheSize)
   {
      setMaxCacheSize(cacheSize);
   }
   else
   {
      setMaxCacheSize(DEFAULT_SIZE);
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG: cache tile size = " << theTileSize << std::endl
         << "Cache size = " << cacheSize << " bytes" << std::endl;
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAppFixedTileCache::ossimAppFixedTileCache() DEBUG: leaving ..." << std::endl;
   }
}

ossimAppFixedTileCache::~ossimAppFixedTileCache()
{
   deleteAll();
}

ossimAppFixedTileCache *ossimAppFixedTileCache::instance(ossim_uint32  maxSize)
{
   if(!theInstance)
   {
      theInstance = new ossimAppFixedTileCache;
      if(maxSize)
	{
	  theInstance->setMaxCacheSize(maxSize);
	}
   }
   return theInstance;
}

void ossimAppFixedTileCache::setMaxCacheSize(ossim_uint32 cacheSize)
{
OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   theMaxGlobalCacheSize = cacheSize;
   theMaxCacheSize = cacheSize;
   //   theMaxCacheSize      = (ossim_uint32)(theMaxGlobalCacheSize*.2);
}

void ossimAppFixedTileCache::flush()
{
OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   std::map<ossimAppFixedCacheId, ossimFixedTileCache*>::iterator currentIter = theAppCacheMap.begin();

   while(currentIter != theAppCacheMap.end())
   {
      (*currentIter).second->flush();
      ++currentIter;
   }
   theCurrentCacheSize = 0;
}

void ossimAppFixedTileCache::flush(ossimAppFixedCacheId cacheId)
{
OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimFixedTileCache* cache = getCache(cacheId);

   if(cache)
   {
      theCurrentCacheSize -= cache->getCacheSize();
      cache->flush();
   }
}

void ossimAppFixedTileCache::deleteCache(ossimAppFixedCacheId cacheId)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimFixedTileCache* cache = getCache(cacheId);
   std::map<ossimAppFixedCacheId, ossimFixedTileCache*>::iterator iter = theAppCacheMap.find(cacheId);

   if(cache)
   {
      theAppCacheMap.erase(iter);
      theCurrentCacheSize -= cache->getCacheSize();
      delete cache;
   }
}

ossimAppFixedTileCache::ossimAppFixedCacheId ossimAppFixedTileCache::newTileCache(const ossimIrect& tileBoundaryRect,
                                                                                  const ossimIpt& tileSize)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimAppFixedCacheId result = theUniqueAppIdCounter;
   ossimFixedTileCache* newCache = new ossimFixedTileCache;
   if(tileSize.x == 0 ||
      tileSize.y == 0)
   {
      // newCache->setRect(tileBoundaryRect, theTileSize);
      newCache->setRect(tileBoundaryRect,
                        newCache->getTileSize());
   }
   else
   {
      newCache->setRect(tileBoundaryRect, tileSize);
   }
   
   theAppCacheMap.insert(std::make_pair(result, newCache));
   ++theUniqueAppIdCounter;
   
   return result;
}

ossimAppFixedTileCache::ossimAppFixedCacheId ossimAppFixedTileCache::newTileCache()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimAppFixedCacheId result = theUniqueAppIdCounter;
   ossimFixedTileCache* newCache = new ossimFixedTileCache;
   
   theAppCacheMap.insert(std::make_pair(result, newCache));
   ++theUniqueAppIdCounter;
   
   return result;
   
}

void ossimAppFixedTileCache::setRect(ossimAppFixedCacheId cacheId,
                                     const ossimIrect& boundaryTileRect)
{
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
      
      ossim_uint32 cacheSize = cache->getCacheSize();
      // cache->setRect(boundaryTileRect, theTileSize);
      cache->setRect(boundaryTileRect,
                     cache->getTileSize());      
      theCurrentCacheSize += (cache->getCacheSize() - cacheSize);
   }
}

void ossimAppFixedTileCache::setTileSize(ossimAppFixedCacheId cacheId,
                                         const ossimIpt& tileSize)
{
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
      ossim_uint32 cacheSize = cache->getCacheSize();
      cache->setRect(cache->getTileBoundaryRect(), tileSize);
      theCurrentCacheSize += (cache->getCacheSize() - cacheSize);
      theTileSize = cache->getTileSize();
   }
}

ossimRefPtr<ossimImageData> ossimAppFixedTileCache::getTile(
   ossimAppFixedCacheId cacheId,
   const ossimIpt& origin)
{
   ossimRefPtr<ossimImageData> result = 0;
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
      result = cache->getTile(origin);
   }

   return result;
}


ossimRefPtr<ossimImageData> ossimAppFixedTileCache::addTile(
   ossimAppFixedCacheId cacheId,
   ossimRefPtr<ossimImageData> data)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimRefPtr<ossimImageData> result = 0;
   ossimFixedTileCache *aCache = this->getCache(cacheId);
   if(!aCache)
   {         
      return result;
   }
   ossim_uint32 dataSize = data->getDataSizeInBytes();

   if( (theCurrentCacheSize+dataSize) > theMaxGlobalCacheSize)
   {
      shrinkGlobalCacheSize((ossim_int32)(theMaxGlobalCacheSize*0.1));
   }

   ossim_uint32 cacheSize = aCache->getCacheSize();
   if(cacheSize > theMaxCacheSize)
   {
//       shrinkCacheSize(aCache,
//                       (ossim_int32)(aCache->getCacheSize()*.1));
      shrinkCacheSize(aCache,
                      (ossim_int32)(1024*1024));
   }
   cacheSize = aCache->getCacheSize();
   aCache->addTile(data);
   theCurrentCacheSize += (aCache->getCacheSize() - cacheSize);
   
   return result;
}

void ossimAppFixedTileCache::deleteAll()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   std::map<ossimAppFixedCacheId, ossimFixedTileCache*>::iterator iter = theAppCacheMap.begin();
   theCurrentCacheSize = 0;

   while(iter != theAppCacheMap.end())
   {
      if((*iter).second)
      {
         delete (*iter).second;
      }
      
      ++iter;
   }
   theAppCacheMap.clear();
}

ossimRefPtr<ossimImageData> ossimAppFixedTileCache::removeTile(
   ossimAppFixedCacheId cacheId,
   const ossimIpt& origin)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimRefPtr<ossimImageData> result = 0;
   
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
      ossim_uint32 cacheSize = cache->getCacheSize();
      result = cache->removeTile(origin);
      theCurrentCacheSize += (cache->getCacheSize() - cacheSize);
   }

   return result;
}

void ossimAppFixedTileCache::deleteTile(ossimAppFixedCacheId cacheId,
                                        const ossimIpt& origin)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
      ossim_uint32 cacheSize = cache->getCacheSize();
      cache->deleteTile(origin);
      theCurrentCacheSize += (cache->getCacheSize() - cacheSize);
   }
}

ossimFixedTileCache* ossimAppFixedTileCache::getCache(ossimAppFixedCacheId cacheId)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   std::map<ossimAppFixedCacheId, ossimFixedTileCache*>::iterator currentIter = theAppCacheMap.find(cacheId);
   ossimFixedTileCache* result = 0;
   
   if(currentIter != theAppCacheMap.end())
   {
      result = (*currentIter).second;
   }

   return result;
}

void ossimAppFixedTileCache::shrinkGlobalCacheSize(ossim_int32 byteCount)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(static_cast<ossim_uint32>(byteCount) >= theCurrentCacheSize)
   {
      flush();
   }
   else
   {
      while(byteCount > 0)
      {
         std::map<ossimAppFixedCacheId, ossimFixedTileCache*>::iterator iter = theAppCacheMap.begin();
         while( (iter != theAppCacheMap.end())&&(byteCount>0))
         {
            ossimFixedTileCache* cache = getCache((*iter).first);
            if(cache)
            {
               ossim_uint32 before = cache->getCacheSize();
               cache->deleteTile();
               ossim_uint32 after = cache->getCacheSize();
               ossim_uint32 delta = (before - after);
               byteCount -= delta;
               theCurrentCacheSize -= (delta);
               ++iter;
            }
         }
      }
   }
}

void ossimAppFixedTileCache::shrinkCacheSize(ossimAppFixedCacheId id,
                                             ossim_int32 byteCount)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimFixedTileCache* cache = getCache(id);

   if(cache)
   {
      shrinkCacheSize(cache, byteCount);
   }
}

void ossimAppFixedTileCache::shrinkCacheSize(ossimFixedTileCache* cache,
                                             ossim_int32 byteCount)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(cache)
   {
      ossim_int32 cacheSize = cache->getCacheSize();
      if(cacheSize <= byteCount)
      {
         cache->flush();
      }
      else
      {
         while(byteCount > 0)
         {
            ossim_uint32 before = cache->getCacheSize();
            cache->deleteTile();
            ossim_uint32 after = cache->getCacheSize();
            ossim_uint32 delta = std::abs((int)(before - after));
            if(delta)
            {
               byteCount -= delta;
               theCurrentCacheSize -= (delta);
            }
            else
            {
               byteCount = 0;
            }
         }
      }
   }
}

const ossimIpt& ossimAppFixedTileCache::getTileSize(ossimAppFixedCacheId cacheId)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimFixedTileCache* cache = getCache(cacheId);
   if(cache)
   {
      return cache->getTileSize();
   }
   return theTileSize;
}
