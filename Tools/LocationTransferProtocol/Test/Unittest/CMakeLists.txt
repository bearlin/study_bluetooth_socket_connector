project(LocationTransferProtocol_Test_Unittest)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(LocationTransferProtocol_Test_Unittest_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol_Test_Unittest_ROOT root directory")
  #message(STATUS "*** LocationTransferProtocol_Test_Unittest_ROOT = ${LocationTransferProtocol_Test_Unittest_ROOT}")

set(LocationTransferProtocol_Test_Unittest_API_INCLUDES
  ${CMAKE_CURRENT_LIST_DIR}/MockInterface
  CACHE STRING "LocationTransferProtocol_Test_Unittest_API_INCLUDES path")
  #message(STATUS "*** using variable 'LocationTransferProtocol_Test_Unittest_API_INCLUDES'=${LocationTransferProtocol_Test_Unittest_API_INCLUDES}")

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Common
  ${CMAKE_CURRENT_BINARY_DIR}/Common
)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Connection
  ${CMAKE_CURRENT_BINARY_DIR}/Connection
)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/RpcOverSpp
  ${CMAKE_CURRENT_BINARY_DIR}/RpcOverSpp
)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/UserDataConnector
  ${CMAKE_CURRENT_BINARY_DIR}/UserDataConnector
)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/UserDataService
  ${CMAKE_CURRENT_BINARY_DIR}/UserDataService
)

# List of LocationTransferProtocol_Unittest targets
list(APPEND LocationTransferProtocol_Unittest_LIST
  LocationTransferProtocol_Test_Unittest_Common
  LocationTransferProtocol_Test_Unittest_Connection
  LocationTransferProtocol_Test_Unittest_RpcOverSpp
  LocationTransferProtocol_Test_Unittest_UserDataConnector
  LocationTransferProtocol_Test_Unittest_UserDataService
)
# Add this project as a custom target, and add its dependency targets.
add_custom_target(${PROJECT_NAME} DEPENDS ${LocationTransferProtocol_Unittest_LIST} COMMENT "LocationTransferProtocol_Test_Unittest custom target")





