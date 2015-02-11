project(HciSimpleScan)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BTServices_Tools_HciSimpleScan_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices_Tools_HciSimpleScan root directory")
  #message(STATUS "*** BTServices_Tools_HciSimpleScan_ROOT = ${BTServices_Tools_HciSimpleScan_ROOT}")
  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BTServices_Tools_HciSimpleScan_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    CACHE STRING "BTServices_Tools_HciSimpleScan_API_INCLUDES path")
  #message(STATUS "*** using variable 'BTServices_Tools_HciSimpleScan_API_INCLUDES'=${BTServices_Tools_HciSimpleScan_API_INCLUDES}")

# Make a list of the source files we use.
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.cpp)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.h)
  #message(">>>>>  SOURCE_FILES = ${SOURCE_FILES}")
  #message(">>>>>  HEADER_FILES = ${HEADER_FILES}")

# Specify resulting executable.
add_executable( ${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Specify include directories.
include_directories( ${CMAKE_CURRENT_LIST_DIR}/Implementation/)
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
include_directories(
    ${BTServices_Tools_HciSimpleScan_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_librapidjson_API_INCLUDES'=${ThirdParty_rapidjson_librapidjson_API_INCLUDES}")

#set_property( TARGET ${PROJECT_NAME} PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})

# Specify link libs.
find_library(BLUEZ_LIB bluetooth)
if (${BLUEZ_LIB} STREQUAL "BLUEZ_LIB-NOTFOUND")
  message( FATAL_ERROR "### ERROR ### Not found variable 'BLUEZ_LIB'.")
else()
  message(STATUS "*** using variable 'BLUEZ_LIB'=${BLUEZ_LIB}")
endif()
target_link_libraries(${PROJECT_NAME} ${BLUEZ_LIB})
target_link_libraries(${PROJECT_NAME} rapidjson)

