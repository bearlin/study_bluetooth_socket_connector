project(LocationTransferProtocol_Imports)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(LocationTransferProtocol_Imports_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "LocationTransferProtocol_Imports_ROOT root directory")
  message(STATUS "*** LocationTransferProtocol_Imports_ROOT = ${LocationTransferProtocol_Imports_ROOT}")

# Logging
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Logging
  ${CMAKE_CURRENT_BINARY_DIR}/Logging
)
  
#if(UNIT_TESTS)
  add_subdirectory(
	  ${CMAKE_CURRENT_SOURCE_DIR}/Unittest
	  ${CMAKE_CURRENT_BINARY_DIR}/Unittest
  )
#endif(UNIT_TESTS)

# Add this project as a custom target, and add its dependency targets.
add_custom_target(${PROJECT_NAME} COMMENT "LocationTransferProtocol_Imports custom target")
add_dependencies(${PROJECT_NAME} LocationTransferProtocol_Imports_Logging)
add_dependencies(${PROJECT_NAME} LocationTransferProtocol_Imports_Unittest)
