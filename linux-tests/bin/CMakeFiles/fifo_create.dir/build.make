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
include CMakeFiles/fifo_create.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fifo_create.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fifo_create.dir/flags.make

CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o: CMakeFiles/fifo_create.dir/flags.make
CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o: ../fifo/fifo_create.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o -c /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/fifo/fifo_create.cpp

CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/fifo/fifo_create.cpp > CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.i

CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/fifo/fifo_create.cpp -o CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.s

# Object files for target fifo_create
fifo_create_OBJECTS = \
"CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o"

# External object files for target fifo_create
fifo_create_EXTERNAL_OBJECTS =

../fifo_create: CMakeFiles/fifo_create.dir/fifo/fifo_create.cpp.o
../fifo_create: CMakeFiles/fifo_create.dir/build.make
../fifo_create: CMakeFiles/fifo_create.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../fifo_create"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fifo_create.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fifo_create.dir/build: ../fifo_create

.PHONY : CMakeFiles/fifo_create.dir/build

CMakeFiles/fifo_create.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fifo_create.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fifo_create.dir/clean

CMakeFiles/fifo_create.dir/depend:
	cd /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin /mnt/c/Users/Diana/Documents/OneDrive/OS/project/lib/linux-tests/bin/CMakeFiles/fifo_create.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fifo_create.dir/depend

