# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/nwuking/netlib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nwuking/netlib/build

# Include any dependencies generated for this target.
include netlib/net/tests/CMakeFiles/buffer_test.dir/depend.make

# Include the progress variables for this target.
include netlib/net/tests/CMakeFiles/buffer_test.dir/progress.make

# Include the compile flags for this target's objects.
include netlib/net/tests/CMakeFiles/buffer_test.dir/flags.make

netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.o: netlib/net/tests/CMakeFiles/buffer_test.dir/flags.make
netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.o: ../netlib/net/tests/Buffer_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nwuking/netlib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.o"
	cd /home/nwuking/netlib/build/netlib/net/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/buffer_test.dir/Buffer_test.cc.o -c /home/nwuking/netlib/netlib/net/tests/Buffer_test.cc

netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buffer_test.dir/Buffer_test.cc.i"
	cd /home/nwuking/netlib/build/netlib/net/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nwuking/netlib/netlib/net/tests/Buffer_test.cc > CMakeFiles/buffer_test.dir/Buffer_test.cc.i

netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buffer_test.dir/Buffer_test.cc.s"
	cd /home/nwuking/netlib/build/netlib/net/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nwuking/netlib/netlib/net/tests/Buffer_test.cc -o CMakeFiles/buffer_test.dir/Buffer_test.cc.s

# Object files for target buffer_test
buffer_test_OBJECTS = \
"CMakeFiles/buffer_test.dir/Buffer_test.cc.o"

# External object files for target buffer_test
buffer_test_EXTERNAL_OBJECTS =

bin/buffer_test: netlib/net/tests/CMakeFiles/buffer_test.dir/Buffer_test.cc.o
bin/buffer_test: netlib/net/tests/CMakeFiles/buffer_test.dir/build.make
bin/buffer_test: lib/libnetlib_net.a
bin/buffer_test: lib/libnetlib_base.a
bin/buffer_test: netlib/net/tests/CMakeFiles/buffer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nwuking/netlib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/buffer_test"
	cd /home/nwuking/netlib/build/netlib/net/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/buffer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
netlib/net/tests/CMakeFiles/buffer_test.dir/build: bin/buffer_test

.PHONY : netlib/net/tests/CMakeFiles/buffer_test.dir/build

netlib/net/tests/CMakeFiles/buffer_test.dir/clean:
	cd /home/nwuking/netlib/build/netlib/net/tests && $(CMAKE_COMMAND) -P CMakeFiles/buffer_test.dir/cmake_clean.cmake
.PHONY : netlib/net/tests/CMakeFiles/buffer_test.dir/clean

netlib/net/tests/CMakeFiles/buffer_test.dir/depend:
	cd /home/nwuking/netlib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nwuking/netlib /home/nwuking/netlib/netlib/net/tests /home/nwuking/netlib/build /home/nwuking/netlib/build/netlib/net/tests /home/nwuking/netlib/build/netlib/net/tests/CMakeFiles/buffer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : netlib/net/tests/CMakeFiles/buffer_test.dir/depend

