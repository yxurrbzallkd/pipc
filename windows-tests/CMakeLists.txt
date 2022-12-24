cmake_minimum_required(VERSION 3.6)
set(PIPC_LIB pipc)
project(pipc
	VERSION 1.0
	LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
add_library(${PIPC_LIB} INTERFACE)

target_include_directories(${PIPC_LIB} 
		INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../windows)


set(process_tests basic_process_test file_forward_test piped_process_test run_grab_test)

add_executable(hello process/hello.cpp) # used for testing
target_link_libraries(hello PUBLIC ${PIPC_LIB})

foreach(t ${process_tests})
add_executable(${t} process/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

set(fifo_tests test)

foreach(t ${fifo_tests})
add_executable(${t} fifo/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

set(shmem_tests test)

foreach(t ${shmem_tests})
add_executable(${t} shmem/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

