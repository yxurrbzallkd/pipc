cmake_minimum_required(VERSION 3.16)
set(PIPC_LIB pipc)
project(pipc
	VERSION 1.0
	LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_library(${PIPC_LIB} INTERFACE)

target_include_directories(${PROJECT_NAME}
			INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../linux)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

#set(process_tests basic_process_test file_forward_test piped_process_test run_grab_test background_test)

add_executable(hello process/hello.cpp) # used for testing

add_executable(infinite process/infinite.cpp) # used for testing


foreach(t ${process_tests})
add_executable(${t} ./process/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

#set(fifo_tests fifo_test_wr fifo_test_fork_wr  fifo_test_rw fifo_test_fork_rw fifo_create
#				spare_test_wr wr_child spare_test_rw rw_child)

foreach(t ${fifo_tests})
add_executable(${t} ./fifo/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

set(shmem_tests shmem_test_single shmem_test_rw shmem_test_wr)

foreach(t ${shmem_tests})
add_executable(${t} ./shmem/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB} -lrt)
endforeach()

#set(socket_tests socket_basic_test)

foreach(t ${socket_tests})
add_executable(${t} ./socket/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()
	