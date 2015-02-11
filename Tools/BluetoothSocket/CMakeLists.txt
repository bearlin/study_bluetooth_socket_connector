set(BTServices_Tools_BluetoothSocket_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices Tools BluetoothSocket root directory")
  #message(STATUS "*** BTServices_Tools_BluetoothSocket_ROOT = ${BTServices_Tools_BluetoothSocket_ROOT}")
  
# RFCOMM 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/RFCOMM 
	${CMAKE_CURRENT_BINARY_DIR}/RFCOMM)
