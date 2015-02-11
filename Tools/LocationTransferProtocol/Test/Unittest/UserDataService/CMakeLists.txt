project(LocationTransferProtocol_Test_Unittest_UserDataService)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(LocationTransferProtocol_Test_Unittest_UserDataService_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol_Test_Unittest_UserDataService_ROOT root directory")
  #message(STATUS "*** LocationTransferProtocol_Test_Unittest_UserDataService_ROOT = ${LocationTransferProtocol_Test_Unittest_UserDataService_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify include directories.
include_directories(
    ${LocationTransferProtocol_Test_Unittest_API_INCLUDES}
    ${LocationTransferProtocol_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    ${ThirdParty_googlemock_libgtest_API_INCLUDES}
    ${ThirdParty_googlemock_libgmock_API_INCLUDES} 
    )

# Core tests
file(GLOB UNITTEST_FILES_CPP ${LocationTransferProtocol_Test_Unittest_UserDataService_ROOT}/*.cpp)
file(GLOB UNITTEST_FILES_H ${LocationTransferProtocol_Test_Unittest_UserDataService_ROOT}/*.h)
  #message(">>>>>  UNITTEST_FILES_CPP = ${UNITTEST_FILES_CPP}")
  #message(">>>>>  UNITTEST_FILES_H = ${UNITTEST_FILES_H}")
  
#append all source files
list(APPEND COMMON_UNITTEST_FILES
	${UNITTEST_FILES_CPP}
	${UNITTEST_FILES_H}
	)

# Test executable
add_executable(${PROJECT_NAME} ${COMMON_UNITTEST_FILES})

target_link_libraries(${PROJECT_NAME}
    LocationTransferProtocol
    gmock_main
    )
