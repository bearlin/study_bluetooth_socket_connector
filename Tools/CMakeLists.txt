set(BTServices_Tools_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices Tools root directory")
  #message(STATUS "*** BTServices_Tools_ROOT = ${BTServices_Tools_ROOT}")
  
# ThirdParty 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty 
	${CMAKE_CURRENT_BINARY_DIR}/ThirdParty)
  
# HciSimpleScan 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/HciSimpleScan 
	${CMAKE_CURRENT_BINARY_DIR}/HciSimpleScan)
  
# BluetoothSocket 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/BluetoothSocket 
	${CMAKE_CURRENT_BINARY_DIR}/BluetoothSocket)
  
# RJDoc 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/RJDoc 
	${CMAKE_CURRENT_BINARY_DIR}/RJDoc)

# LocationTransferProtocol 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/LocationTransferProtocol 
	${CMAKE_CURRENT_BINARY_DIR}/LocationTransferProtocol)