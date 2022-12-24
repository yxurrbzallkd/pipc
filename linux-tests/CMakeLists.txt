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

set(process_tests basic_process_test file_forward_test piped_process_test run_grab_test)

add_executable(hello process/hello.cpp) # used for testing
target_link_libraries(hello PUBLIC ${PIPC_LIB})

foreach(t ${process_tests})
add_executable(${t} ./process/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

set(fifo_tests fifo_test_wr fifo_test_fork_wr fifo_test_wait_wr
				fifo_test_rw fifo_test_fork_rw fifo_test_wait_rw)

foreach(t ${fifo_tests})
add_executable(${t} ./fifo/${t}.cpp)
target_link_libraries(${t} PUBLIC ${PIPC_LIB})
endforeach()

set(shmem_tests shmem_test)

foreach(t ${shmem_tests})
add_executable(${t} ./shmem/${t}.cpp)
message(./shmem/${t}.cpp)
#target_compile_options(${t} PUBLIC -lrt)
target_link_libraries(${t} PUBLIC ${PIPC_LIB} -lrt)
endforeach()

	