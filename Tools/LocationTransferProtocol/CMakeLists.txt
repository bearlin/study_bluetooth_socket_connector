project(LocationTransferProtocol)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

# Imports
add_subdirectory(
        ${PROJECT_SOURCE_DIR}/Imports
        ${PROJECT_BINARY_DIR}/Imports)
        
set(LocationTransferProtocol_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol root directory")
  #message(STATUS "*** LocationTransferProtocol_ROOT = ${LocationTransferProtocol_ROOT}")
  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LocationTransferProtocol_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    CACHE STRING "LocationTransferProtocol_API_INCLUDES path")
  #message(STATUS "*** using variable 'LocationTransferProtocol_API_INCLUDES'=${LocationTransferProtocol_API_INCLUDES}")

CmakeCheckVar(ThirdParty_rapidjson_librapidjson_API_INCLUDES)
  #message(FATAL_ERROR ">>>>>  ThirdParty_rapidjson_librapidjson_API_INCLUDES = '${ThirdParty_rapidjson_librapidjson_API_INCLUDES}'")

CmakeCheckVar( LocationTransferProtocol_Imports_Logging_API_INCLUDES)

# Specify include directories.
include_directories(
    ${LocationTransferProtocol_Imports_Logging_API_INCLUDES}
    ${LocationTransferProtocol_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'ProjThirdParty_librapidjson_API_INCLUDES'=${ProjThirdParty_librapidjson_API_INCLUDES}")

# Make a list of the source files we use.
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.cpp)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/Interface/*.h)
  #message(">>>>>  SOURCE_FILES = ${SOURCE_FILES}")
  #message(">>>>>  HEADER_FILES = ${HEADER_FILES}")

# Specify resulting library.
add_library( ${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# We need bluez to use Bluetooth Socket Server/Client.
find_library(BLUEZ_LIB bluetooth)
if (${BLUEZ_LIB} STREQUAL "BLUEZ_LIB-NOTFOUND")
  message(FATAL_ERROR "### ERROR ### Not found variable 'BLUEZ_LIB'. Please using 'sudo apt-get install libbluetooth-dev:i386' to install package.")
else()
  message(STATUS "*** using variable 'BLUEZ_LIB'=${BLUEZ_LIB}")
endif()

# We need pthread.
find_library(PTHREAD_LIB pthread)
if (${PTHREAD_LIB} STREQUAL "PTHREAD_LIB-NOTFOUND")
  message(FATAL_ERROR "### ERROR ### Not found variable 'PTHREAD_LIB'.")
else()
  message(STATUS "*** using variable 'PTHREAD_LIB'=${PTHREAD_LIB}")
endif()

# We need rt, the POSIX.1b Realtime Extensions library.
find_library(REALTIME_LIB rt)
if (${REALTIME_LIB} STREQUAL "REALTIME_LIB-NOTFOUND")
  message(FATAL_ERROR "### ERROR ### Not found variable 'REALTIME_LIB'.")
else()
  message(STATUS "*** using variable 'REALTIME_LIB'=${REALTIME_LIB}")
endif()

target_link_libraries(${PROJECT_NAME} 
                      rapidjson
                      LocationTransferProtocol_Imports_Logging
                      ${BLUEZ_LIB}
                      ${PTHREAD_LIB}
                      ${REALTIME_LIB}
                      )

#if(UNIT_TESTS)
  add_subdirectory(
	  ${CMAKE_CURRENT_SOURCE_DIR}/Test
	  ${CMAKE_CURRENT_BINARY_DIR}/Test
  )
#endif(UNIT_TESTS)

