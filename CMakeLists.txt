cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME pipc)
project(pipc
	VERSION 1.0
	LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
message("======================\n" "Host system is:" ${CMAKE_HOST_SYSTEM_NAME} "\n======================\n")

add_library(${PROJECT_NAME} INTERFACE)

if(WIN32)
	message("Windows Here!")
	target_include_directories(${PROJECT_NAME} 
		INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/windows)
elseif (UNIX AND NOT APPLE)
	if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
		message("Linux Here!")
		target_include_directories(${PROJECT_NAME}
			INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/linux)
	endif()
	if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Minix")
		message("Minix Here!")	
		target_include_directories(${PROJECT_NAME}
			INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/linux)
	endif()
else()
	message(FATAL_ERROR ${CMAKE_HOST_SYSTEM_NAME} "not supported")
endif()

add_executable(hello demo.cpp)
target_link_libraries(hello PUBLIC ${PROJECT_NAME})
