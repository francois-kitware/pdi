# - Try to find hwloc
# Once done this will define
#
#  HWLOC_FOUND - system has hwloc
#  HWLOC_INCLUDE_DIR - the hwloc include directory
#  HWLOC_LIBRARIES - Link these to use hwloc
#  HWLOC_DEFINITIONS - Compiler switches required for using hwloc
#


FIND_PATH(HWLOC_INCLUDE_DIR NAMES hwloc.h
  PATHS
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
  ENV CPATH
  /usr/include
  /usr/local/include
  NO_DEFAULT_PATH
)
FIND_PATH(HWLOC_INCLUDE_DIR NAMES hwloc.h)

FIND_LIBRARY(HWLOC_LIBRARIES NAMES hwloc
  PATHS
  ${PROJECT_BINARY_DIR}/lib64
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib64
  ${PROJECT_SOURCE_DIR}/lib
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib64
  /usr/lib
  /usr/local/lib64
  /usr/local/lib
  /usr/lib/x86_64-linux-gnu/
  NO_DEFAULT_PATH
)
FIND_LIBRARY(HWLOC_LIBRARIES NAMES hwloc)

IF(HWLOC_INCLUDE_DIR AND HWLOC_LIBRARIES)
   SET(HWLOC_FOUND TRUE)
ENDIF(HWLOC_INCLUDE_DIR AND HWLOC_LIBRARIES)

IF(HWLOC_FOUND)
  IF(NOT HWLOC_FIND_QUIETLY)
    MESSAGE(STATUS "Found hwloc: ${HWLOC_LIBRARIES}")
  ENDIF(NOT HWLOC_FIND_QUIETLY)
ELSE(HWLOC_FOUND)
  IF(HWLOC_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find hwloc")
  ENDIF(HWLOC_FIND_REQUIRED)
ENDIF(HWLOC_FOUND)

# show the HWLOC_INCLUDE_DIR and HWLOC_LIBRARIES variables only in the advanced view
  MARK_AS_ADVANCED(HWLOC_INCLUDE_DIR HWLOC_LIBRARIES )
