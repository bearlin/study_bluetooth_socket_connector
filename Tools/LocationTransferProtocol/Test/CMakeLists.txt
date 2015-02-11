project(LocationTransferProtocol_Test)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(LocationTransferProtocol_Test_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol_Test_ROOT root directory")
  #message(STATUS "*** LocationTransferProtocol_Test_ROOT = ${LocationTransferProtocol_Test_ROOT}")

# Demo
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Demo
  ${CMAKE_CURRENT_BINARY_DIR}/Demo
)
  
#if(UNIT_TESTS)
  add_subdirectory(
	  ${CMAKE_CURRENT_SOURCE_DIR}/Unittest
	  ${CMAKE_CURRENT_BINARY_DIR}/Unittest
  )
#endif(UNIT_TESTS)

# Add this project as a custom target, and add its dependency targets.
add_custom_target(${PROJECT_NAME} COMMENT "LocationTransferProtocol_Test custom target")
add_dependencies(${PROJECT_NAME} LocationTransferProtocol_Test_Demo)
add_dependencies(${PROJECT_NAME} LocationTransferProtocol_Test_Unittest)
