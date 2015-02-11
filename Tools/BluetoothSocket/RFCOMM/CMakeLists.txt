project(BT_Socket)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BTServices_Tools_BluetoothSocket_RFCOMM_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices_Tools_BluetoothSocket_RFCOMM root directory")
  #message(STATUS "*** BTServices_Tools_BluetoothSocket_RFCOMM_ROOT = ${BTServices_Tools_BluetoothSocket_RFCOMM_ROOT}")
  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BTServices_Tools_BluetoothSocket_RFCOMM_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    CACHE STRING "BTServices_Tools_BluetoothSocket_RFCOMM_API_INCLUDES path")
  #message(STATUS "*** using variable 'BTServices_Tools_BluetoothSocket_RFCOMM_API_INCLUDES'=${BTServices_Tools_BluetoothSocket_RFCOMM_API_INCLUDES}")

# Make a list of the source files we use.
file(GLOB SOURCE_FILES_SERVER ${CMAKE_CURRENT_LIST_DIR}/Implementation/rfcomm-server.cpp)
file(GLOB SOURCE_FILES_CLIENT ${CMAKE_CURRENT_LIST_DIR}/Implementation/rfcomm-client.cpp)
# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.h)
  #message(">>>>>  SOURCE_FILES_SERVER = ${SOURCE_FILES_SERVER}")
  #message(">>>>>  SOURCE_FILES_CLIENT = ${SOURCE_FILES_CLIENT}")
  #message(">>>>>  HEADER_FILES = ${HEADER_FILES}")

# Include directories
include_directories( ${CMAKE_CURRENT_LIST_DIR}/Implementation/)
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
include_directories(
    ${BTServices_Tools_BluetoothSocket_RFCOMM_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_librapidjson_API_INCLUDES'=${ThirdParty_rapidjson_librapidjson_API_INCLUDES}")

# Specify resulting executable.
add_executable( ${PROJECT_NAME}_Server ${HEADER_FILES} ${SOURCE_FILES_SERVER})
add_executable( ${PROJECT_NAME}_Client ${HEADER_FILES} ${SOURCE_FILES_CLIENT})

#set_property( TARGET ${PROJECT_NAME}_Server PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME}_Server PROPERTY LIBRARY_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME}_Server PROPERTY RUNTIME_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME}_Client PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME}_Client PROPERTY LIBRARY_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})
#set_property( TARGET ${PROJECT_NAME}_Client PROPERTY RUNTIME_OUTPUT_DIRECTORY ${COMPONENT_OUTPUT})

# We need bluez to use Bluetooth Socket Server/Client.
find_library(BLUEZ_LIB bluetooth)
if (${BLUEZ_LIB} STREQUAL "BLUEZ_LIB-NOTFOUND")
  message(FATAL_ERROR "### ERROR ### Not found variable 'BLUEZ_LIB'. Please using 'sudo apt-get install libbluetooth-dev' to install package.")
else()
  message(STATUS "*** using variable 'BLUEZ_LIB'=${BLUEZ_LIB}")
endif()
target_link_libraries( ${PROJECT_NAME}_Server ${BLUEZ_LIB})
target_link_libraries( ${PROJECT_NAME}_Client ${BLUEZ_LIB})
target_link_libraries( ${PROJECT_NAME}_Server rapidjson) 
target_link_libraries( ${PROJECT_NAME}_Client rapidjson)
