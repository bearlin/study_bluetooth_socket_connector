project(RJDoc)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BTServices_Tools_JsonObj_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices_Tools_JsonObj root directory")
  #message(STATUS "*** BTServices_Tools_JsonObj_ROOT = ${BTServices_Tools_JsonObj_ROOT}")
  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BTServices_Tools_JsonObj_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    ${CMAKE_CURRENT_LIST_DIR}/Implementation
    CACHE STRING "BTServices_Tools_JsonObj_API_INCLUDES path")
  #message(STATUS "*** using variable 'BTServices_Tools_JsonObj_API_INCLUDES'=${BTServices_Tools_JsonObj_API_INCLUDES}")

# Make a list of the source files we use.
file(GLOB SOURCE_FILES_RJDoc ${CMAKE_CURRENT_LIST_DIR}/Implementation/RJDoc.cpp)
file(GLOB SOURCE_FILES_RJDocUnittest ${CMAKE_CURRENT_LIST_DIR}/Implementation/RJDocUnittest.cpp)
file(GLOB SOURCE_FILES_UseRJDoc ${CMAKE_CURRENT_LIST_DIR}/Implementation/UseRJDoc.cpp)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES_RJDoc ${CMAKE_CURRENT_LIST_DIR}/Implementation/RJDoc.h)
  #message(">>>>>  SOURCE_FILES_RJDoc = ${SOURCE_FILES_RJDoc}")
  #message(">>>>>  HEADER_FILES_RJDoc = ${HEADER_FILES_RJDoc}")
  #message(">>>>>  SOURCE_FILES_RJDocUnittest = ${SOURCE_FILES_RJDocUnittest}")
  #message(">>>>>  SOURCE_FILES_UseRJDoc = ${SOURCE_FILES_UseRJDoc}")

# Specify resulting executable.
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES_RJDoc} ${HEADER_FILES_RJDoc})
target_link_libraries(${PROJECT_NAME} rapidjson)
add_executable( ${PROJECT_NAME}_unittest ${SOURCE_FILES_RJDocUnittest})
target_link_libraries(${PROJECT_NAME}_unittest RJDoc gtest_main)
add_executable( ${PROJECT_NAME}_UseRJDoc ${SOURCE_FILES_UseRJDoc})
target_link_libraries(${PROJECT_NAME}_UseRJDoc RJDoc)

#disable_warnings_as_errors_on_target(${PROJECT_NAME})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_UseRJDoc)

# Specify include directories.
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
include_directories(
    ${BTServices_Tools_JsonObj_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    ${ThirdParty_googlemock_libgtest_API_INCLUDES}
    ${ThirdParty_googlemock_libgmock_API_INCLUDES} 
    )
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_librapidjson_API_INCLUDES'=${ThirdParty_rapidjson_librapidjson_API_INCLUDES}")
  #message(STATUS "*** using variable 'ThirdParty_googletest_libgtest_API_INCLUDES'=${ThirdParty_googletest_libgtest_API_INCLUDES}")
  #message(STATUS "*** using variable 'ThirdParty_googlemock_libgtest_API_INCLUDES'=${ThirdParty_googlemock_libgtest_API_INCLUDES}")
  #message(STATUS "*** using variable 'ThirdParty_googlemock_libgmock_API_INCLUDES'=${ThirdParty_googlemock_libgmock_API_INCLUDES}")


