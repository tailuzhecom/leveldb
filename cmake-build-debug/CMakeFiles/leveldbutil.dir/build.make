# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/young/下载/CLion-2019.1.4/clion-2019.1.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/young/下载/CLion-2019.1.4/clion-2019.1.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/young/下载/leveldb-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/young/下载/leveldb-master/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/leveldbutil.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/leveldbutil.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/leveldbutil.dir/flags.make

CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o: CMakeFiles/leveldbutil.dir/flags.make
CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o: ../db/leveldbutil.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/young/下载/leveldb-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o -c /home/young/下载/leveldb-master/db/leveldbutil.cc

CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/young/下载/leveldb-master/db/leveldbutil.cc > CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.i

CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/young/下载/leveldb-master/db/leveldbutil.cc -o CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.s

# Object files for target leveldbutil
leveldbutil_OBJECTS = \
"CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o"

# External object files for target leveldbutil
leveldbutil_EXTERNAL_OBJECTS =

leveldbutil: CMakeFiles/leveldbutil.dir/db/leveldbutil.cc.o
leveldbutil: CMakeFiles/leveldbutil.dir/build.make
leveldbutil: libleveldb.a
leveldbutil: CMakeFiles/leveldbutil.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/young/下载/leveldb-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable leveldbutil"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/leveldbutil.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/leveldbutil.dir/build: leveldbutil

.PHONY : CMakeFiles/leveldbutil.dir/build

CMakeFiles/leveldbutil.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/leveldbutil.dir/cmake_clean.cmake
.PHONY : CMakeFiles/leveldbutil.dir/clean

CMakeFiles/leveldbutil.dir/depend:
	cd /home/young/下载/leveldb-master/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/young/下载/leveldb-master /home/young/下载/leveldb-master /home/young/下载/leveldb-master/cmake-build-debug /home/young/下载/leveldb-master/cmake-build-debug /home/young/下载/leveldb-master/cmake-build-debug/CMakeFiles/leveldbutil.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/leveldbutil.dir/depend

