# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin

# Include any dependencies generated for this target.
include CMakeFiles/shm_share_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/shm_share_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shm_share_test.dir/flags.make

CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj: CMakeFiles/shm_share_test.dir/flags.make
CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj: CMakeFiles/shm_share_test.dir/includes_CXX.rsp
CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj: ../shmem/shm_share_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj"
	C:/MINGW/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj -c C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/shmem/shm_share_test.cpp

CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.i"
	C:/MINGW/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/shmem/shm_share_test.cpp > CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.i

CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.s"
	C:/MINGW/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/shmem/shm_share_test.cpp -o CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.s

# Object files for target shm_share_test
shm_share_test_OBJECTS = \
"CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj"

# External object files for target shm_share_test
shm_share_test_EXTERNAL_OBJECTS =

../shm_share_test.exe: CMakeFiles/shm_share_test.dir/shmem/shm_share_test.cpp.obj
../shm_share_test.exe: CMakeFiles/shm_share_test.dir/build.make
../shm_share_test.exe: CMakeFiles/shm_share_test.dir/linklibs.rsp
../shm_share_test.exe: CMakeFiles/shm_share_test.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../shm_share_test.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/shm_share_test.dir/objects.a
	C:/MINGW/bin/ar.exe cr CMakeFiles/shm_share_test.dir/objects.a @CMakeFiles/shm_share_test.dir/objects1.rsp
	C:/MINGW/bin/c++.exe -Wl,--whole-archive CMakeFiles/shm_share_test.dir/objects.a -Wl,--no-whole-archive -o ../shm_share_test.exe -Wl,--out-implib,libshm_share_test.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/shm_share_test.dir/linklibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/shm_share_test.dir/build: ../shm_share_test.exe

.PHONY : CMakeFiles/shm_share_test.dir/build

CMakeFiles/shm_share_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shm_share_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shm_share_test.dir/clean

CMakeFiles/shm_share_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin C:/Users/Diana/Documents/OneDrive/OS/project/lib/windows-tests/bin/CMakeFiles/shm_share_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shm_share_test.dir/depend

