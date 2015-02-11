project(LocationTransferProtocol_Test_Demo)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(LocationTransferProtocol_Test_Demo_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol_Test_Demo_ROOT root directory")
  #message(STATUS "*** LocationTransferProtocol_Test_Demo_ROOT = ${LocationTransferProtocol_Test_Demo_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify include directories.
include_directories(
    ${LocationTransferProtocol_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'LocationTransferProtocol_API_INCLUDES'=${LocationTransferProtocol_API_INCLUDES}")

# Core tests
file(GLOB DEMO_FILES_CPP ${LocationTransferProtocol_Test_Demo_ROOT}/*.cpp)
file(GLOB DEMO_FILES_H ${LocationTransferProtocol_Test_Demo_ROOT}/*.h)
  #message(">>>>>  DEMO_FILES_CPP = ${DEMO_FILES_CPP}")
  #message(">>>>>  DEMO_FILES_H = ${DEMO_FILES_H}")
  
#append all source files
list(APPEND COMMON_DEMO_FILES
	${DEMO_FILES_CPP}
	${DEMO_FILES_H}
	)

# Test executable
add_executable(${PROJECT_NAME} ${COMMON_DEMO_FILES})

target_link_libraries(${PROJECT_NAME}
    LocationTransferProtocol
    )
