# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin

# Include any dependencies generated for this target.
include CMakeFiles/background_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/background_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/background_test.dir/flags.make

CMakeFiles/background_test.dir/process/background_test.cpp.o: CMakeFiles/background_test.dir/flags.make
CMakeFiles/background_test.dir/process/background_test.cpp.o: ../process/background_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/background_test.dir/process/background_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/background_test.dir/process/background_test.cpp.o -c /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/process/background_test.cpp

CMakeFiles/background_test.dir/process/background_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/background_test.dir/process/background_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/process/background_test.cpp > CMakeFiles/background_test.dir/process/background_test.cpp.i

CMakeFiles/background_test.dir/process/background_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/background_test.dir/process/background_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/process/background_test.cpp -o CMakeFiles/background_test.dir/process/background_test.cpp.s

# Object files for target background_test
background_test_OBJECTS = \
"CMakeFiles/background_test.dir/process/background_test.cpp.o"

# External object files for target background_test
background_test_EXTERNAL_OBJECTS =

../background_test: CMakeFiles/background_test.dir/process/background_test.cpp.o
../background_test: CMakeFiles/background_test.dir/build.make
../background_test: CMakeFiles/background_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../background_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/background_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/background_test.dir/build: ../background_test

.PHONY : CMakeFiles/background_test.dir/build

CMakeFiles/background_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/background_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/background_test.dir/clean

CMakeFiles/background_test.dir/depend:
	cd /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles/background_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/background_test.dir/depend

