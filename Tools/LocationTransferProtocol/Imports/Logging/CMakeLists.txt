project(LocationTransferProtocol_Imports_Logging)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")
  
set(LocationTransferProtocol_Imports_Logging_API_INCLUDES
    ${PROJECT_SOURCE_DIR}/Implementation
    CACHE STRING "LocationTransferProtocol_Imports_Logging_API_INCLUDES path")
  message(STATUS "*** using variable 'LocationTransferProtocol_Imports_Logging_API_INCLUDES'=${LocationTransferProtocol_Imports_Logging_API_INCLUDES}")
  
include_directories(
    ${LocationTransferProtocol_Imports_Logging_API_INCLUDES}
)

# Make a list of the source files we use.
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.cpp)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.h)
  message(">>>>>  SOURCE_FILES = ${SOURCE_FILES}")
  message(">>>>>  HEADER_FILES = ${HEADER_FILES}")

# Specify resulting library.
add_library( ${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

