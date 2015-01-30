message(STATUS "Setup OTB...")

set(proj OTB)

set(OTB_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)

set(${proj}_DEPENDENCIES)

set(BUILD_EXAMPLES ON)

if(ENABLE_OTB_LARGE_INPUTS)
  set(OTB_SB_LARGEINPUT_CONFIG 
    -DOTB_DATA_USE_LARGEINPUT:BOOL=ON
    -DOTB_DATA_LARGEINPUT_ROOT:PATH=${OTB_DATA_LARGEINPUT_ROOT}
    )
else()
  set(OTB_SB_LARGEINPUT_CONFIG 
    -DOTB_DATA_USE_LARGEINPUT:BOOL=OFF
    )
endif()

if(ENABLE_QT4)
  if(USE_SYSTEM_QT4)
    set(OTB_SB_WRAP_QT_CONFIG -DOTB_WRAP_QT:BOOL=ON)
  else()
    if(MSVC)
      set(OTB_SB_WRAP_QT_CONFIG -DOTB_WRAP_QT:BOOL=ON)
      list(APPEND ${proj}_DEPENDENCIES QT4)
    else(UNIX)
       #TODO: use must have Qt4 
       message(STATUS "  Qt4 is not built by SuperBuild. You need to install it via package manager.")
      set(OTB_SB_WRAP_QT_CONFIG -DOTB_WRAP_QT:BOOL=OFF)
    endif()
  endif()
else()
  set(OTB_SB_WRAP_QT_CONFIG -DOTB_WRAP_QT:BOOL=OFF)
endif()

if(USE_SYSTEM_GDAL)
  set(OTB_SB_GDAL_CONFIG)
else()
  set(OTB_SB_GDAL_CONFIG
    -DGDAL_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DGDAL_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libgdal${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DGDAL_CONFIG:PATH=${CMAKE_INSTALL_PREFIX}/bin/gdal-config
    )
  list(APPEND ${proj}_DEPENDENCIES GDAL)
endif()

if(USE_SYSTEM_OSSIM)
  set(OTB_SB_OSSIM_CONFIG)
else()
  set(OTB_SB_OSSIM_CONFIG
    -DOSSIM_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DOSSIM_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libossim${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES OSSIM)
endif()

if(USE_SYSTEM_ITK)
  set(OTB_SB_ITK_CONFIG)
else()
  set(OTB_SB_ITK_CONFIG
    -DITK_DIR:PATH=${CMAKE_INSTALL_PREFIX}/lib/cmake/ITK-4.6
    )
  list(APPEND ${proj}_DEPENDENCIES ITK)
endif()

if(USE_SYSTEM_MUPARSER)
  set(OTB_SB_MUPARSER_CONFIG)
else()
  set(OTB_SB_MUPARSER_CONFIG
    -DMUPARSER_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DMUPARSER_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libmuparser${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES MUPARSER)
endif()

if(USE_SYSTEM_MUPARSERX)
  set(OTB_SB_MUPARSERX_CONFIG)
else()
  set(OTB_SB_MUPARSERX_CONFIG
    -DMUPARSERX_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DMUPARSERX_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libmuparserx${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES MUPARSERX)
endif()

if(USE_SYSTEM_TINYXML)
  set(OTB_SB_TINYXML_CONFIG)
else()
  set(OTB_SB_TINYXML_CONFIG
    -DTINYXML_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DTINYXML_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libtinyXML${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES TINYXML)
endif()

if(USE_SYSTEM_BOOST)
  set(OTB_SB_BOOST_CONFIG)
else()
  set(OTB_SB_BOOST_CONFIG
    -DBOOST_ROOT:PATH=${CMAKE_INSTALL_PREFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES BOOST)
endif()

if(USE_SYSTEM_LIBKML)
  set(OTB_SB_LIBKML_CONFIG)
else()
  # TODO : split LIBKML build into 7 libraries
  set(OTB_SB_LIBKML_CONFIG
    -DLIBKML_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DLIBKML_BASE_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmlbase${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_CONVENIENCE_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmlconvenience${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_DOM_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmldom${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_ENGINE_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmlengine${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_REGIONATOR_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmlregionator${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_XSD_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libkmlxsd${CMAKE_SHARED_LIBRARY_SUFFIX}
    -DLIBKML_MINIZIP_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libminizip${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
    
  if(MSVC)
    # expat is a dependency for kmlbase
    set(OTB_SB_LIBKML_CONFIG
    -DLIBKML_INCLUDE_DIR:FILEPATH=${CMAKE_INSTALL_PREFIX}/include
    -DLIBKML_BASE_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmlbase.lib;${CMAKE_INSTALL_PREFIX}/lib/expat.lib
    -DLIBKML_CONVENIENCE_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmlconvenience.lib
    -DLIBKML_DOM_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmldom.lib
    -DLIBKML_ENGINE_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmlengine.lib
    -DLIBKML_REGIONATOR_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmlregionator.lib
    -DLIBKML_XSD_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/kmlxsd.lib
    -DLIBKML_MINIZIP_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/minizip.lib
    )
  endif()
  list(APPEND ${proj}_DEPENDENCIES LIBKML)
endif()

if(USE_SYSTEM_OPENCV)
  set(OTB_SB_OPENCV_CONFIG)
else()
  set(OTB_SB_OPENCV_CONFIG
    -DOpenCV_DIR:PATH=${CMAKE_INSTALL_PREFIX}/share/OpenCV
    )
  list(APPEND ${proj}_DEPENDENCIES OPENCV)
endif()

if(USE_SYSTEM_CURL)
  set(OTB_SB_CURL_CONFIG)
else()
  set(OTB_SB_CURL_CONFIG
    -DCURL_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
    -DCURL_LIBRARY:PATH=${CMAKE_INSTALL_PREFIX}/lib/libcurl${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
  list(APPEND ${proj}_DEPENDENCIES CURL)
endif()

if(MSVC)
  set(BUILD_EXAMPLES OFF)
  set(OTB_SB_GDAL_CONFIG)
  set(OTB_SB_OSSIM_CONFIG)
  set(OTB_SB_MUPARSER_CONFIG)
  set(OTB_SB_MUPARSERX_CONFIG)
  set(OTB_SB_TINYXML_CONFIG)
  set(OTB_SB_OPENCV_CONFIG)
  #doesn't matter to use cmake variable with JPEG prefix. all are empty for windows
  #GDAL is built with internal JPEG due to static link conflict. 
  #replace superbuild version of libjpeg.lib with the GDAL internal one
  set(OTB_SB_CURL_CONFIG -DJPEG_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libjpeg.lib)
endif()

ExternalProject_Add(${proj}
    DEPENDS ${${proj}_DEPENDENCIES}
    PREFIX ${proj}
    HG_REPOSITORY "http://hg.orfeo-toolbox.org/OTB"
    HG_TAG tip
    BINARY_DIR ${OTB_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=${BUILD_TESTING}
      -DBUILD_EXAMPLES:BOOL=${BUILD_EXAMPLES}
      -DBUILD_APPLICATIONS:BOOL=ON
      -DCMAKE_PREFIX_PATH:STRING=${CMAKE_INSTALL_PREFIX}
      -DOTB_DATA_ROOT:STRING=${OTB_DATA_ROOT}
      -DOTB_USE_EXTERNAL_OSSIM:BOOL=ON
      -DOTB_USE_EXTERNAL_BOOST:BOOL=ON
      -DOTB_USE_EXTERNAL_ITK:BOOL=ON
      -DOTB_USE_EXTERNAL_MUPARSER:BOOL=ON
      -DOTB_USE_EXTERNAL_MUPARSERX:BOOL=ON
      -DOTB_USE_EXTERNAL_TINYXML:BOOL=ON
      -DOTB_USE_EXTERNAL_EXPAT:BOOL=ON
      -DOTB_USE_EXTERNAL_LIBKML:BOOL=ON
      -DOTB_USE_OPENCV:BOOL=ON
      -DOTB_USE_JPEG2000:BOOL=OFF
      ${OTB_SB_WRAP_QT_CONFIG}
      ${OTB_SB_ITK_CONFIG}
      ${OTB_SB_OSSIM_CONFIG}
      ${OTB_SB_GDAL_CONFIG}
      ${OTB_SB_MUPARSER_CONFIG}
      ${OTB_SB_MUPARSERX_CONFIG}
      ${OTB_SB_TINYXML_CONFIG}
      ${OTB_SB_BOOST_CONFIG}
      ${OTB_SB_LIBKML_CONFIG}
      ${OTB_SB_OPENCV_CONFIG}
      ${OTB_SB_CURL_CONFIG}
      ${OTB_SB_LARGEINPUT_CONFIG}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )

message(STATUS "  Using OTB SuperBuild version")
