cmake_minimum_required (VERSION 2.8.11)
project (consoletools)

#====================================================
# user settings
#====================================================
set(COLORER_FEATURE_HTTPINPUTSOURCE OFF CACHE BOOL "If defined, HTTP InputSource is implemented")

#====================================================
# init variables
#====================================================
  
#====================================================
# colorer_lib
#====================================================
include_directories (../../src/)
include_directories (../../libs/xercesc/src)
add_definitions(-DXERCES_STATIC_LIBRARY)


set(SRC_CPP ConsoleToolsRunner.cpp
   ConsoleTools.cpp ConsoleTools.h
   version.h)

if(WIN32)
  set(SRC_CPP ${SRC_CPP} ConsoleToolsRunner.rc)
endif(WIN32)

#====================================================
# common flags
#====================================================

if (COLORER_FEATURE_HTTPINPUTSOURCE AND WIN32)
  set(WININETLIB wininet)
endif()

if(WIN32)
  add_definitions(-D_WINDOWS)
  add_definitions(-D_WIN32)
  if(${CMAKE_CL_64})
    add_definitions(-D_WIN64)
  endif()
endif(WIN32)

add_definitions(-D_UNICODE -DUNICODE)

set(FLAGS_DEBUG "-D_DEBUG")

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

  set(COLORER_WARNING_FLAGS "")
  set(FLAGS_CXX_DEFAULT "${FLAGS_CXX_DEFAULT} /EHsc /Zi")
  set(FLAGS_C_DEFAULT "${FLAGS_C_DEFAULT}")

  set(FLAGS_RELEASE "${FLAGS_RELEASE} /O2 /Oi /Ot /Oy /GL /GF /GS- /Gy /MT")
  set(FLAGS_DEBUG "${FLAGS_DEBUG} /Od /MTd /GS /RTC1 /W3")
  set(LINK_FLAGS_RELEASE "/LTCG")

  # support AMD proc on vc2015
  if(${CMAKE_CL_64} STREQUAL "0")
    set(FLAGS_CXX_DEFAULT "${FLAGS_CXX_DEFAULT} /arch:IA32")
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(COLORER_WARNING_FLAGS )
  set(FLAGS_CXX_DEFAULT "${FLAGS_CXX_DEFAULT} -fpermissive -std=c++14")

  set(FLAGS_RELEASE "${FLAGS_RELEASE} -s -O3")
  set(FLAGS_DEBUG "${FLAGS_DEBUG} -g3")
  
  if(NOT(WIN32 OR MINGW OR MSYS))
    INCLUDE(FindThreads)
    if(CMAKE_USE_PTHREADS_INIT)
      set(THREAD_LIBS ${CMAKE_THREAD_LIBS_INIT})
    endif()
  endif()
endif()

set(CMAKE_C_FLAGS "${FLAGS_C_DEFAULT} ${COLORER_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${FLAGS_CXX_DEFAULT} ${COLORER_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE "${FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG "${FLAGS_DEBUG}")

#====================================================
# build
#====================================================

set(LIBRARIES colorer_lib xerces-c_3_1 g3logger)
set(SRC_FILES ${SRC_CPP})
add_executable (colorer ${SRC_FILES} )
target_link_libraries(colorer ${LIBRARIES} ${WININETLIB} ${THREAD_LIBS})
set_target_properties(
  colorer PROPERTIES
  LINK_FLAGS "${LINK_FLAGS}"
  LINK_FLAGS_RELEASE "${LINK_FLAGS_RELEASE}"
)
