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
CMAKE_BINARY_DIR = /home/nwuking/netlib/built

# Include any dependencies generated for this target.
include netlib/base/tests/CMakeFiles/exception_test.dir/depend.make

# Include the progress variables for this target.
include netlib/base/tests/CMakeFiles/exception_test.dir/progress.make

# Include the compile flags for this target's objects.
include netlib/base/tests/CMakeFiles/exception_test.dir/flags.make

netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.o: netlib/base/tests/CMakeFiles/exception_test.dir/flags.make
netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.o: ../netlib/base/tests/exception_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nwuking/netlib/built/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.o"
	cd /home/nwuking/netlib/built/netlib/base/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/exception_test.dir/exception_test.cc.o -c /home/nwuking/netlib/netlib/base/tests/exception_test.cc

netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exception_test.dir/exception_test.cc.i"
	cd /home/nwuking/netlib/built/netlib/base/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nwuking/netlib/netlib/base/tests/exception_test.cc > CMakeFiles/exception_test.dir/exception_test.cc.i

netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exception_test.dir/exception_test.cc.s"
	cd /home/nwuking/netlib/built/netlib/base/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nwuking/netlib/netlib/base/tests/exception_test.cc -o CMakeFiles/exception_test.dir/exception_test.cc.s

# Object files for target exception_test
exception_test_OBJECTS = \
"CMakeFiles/exception_test.dir/exception_test.cc.o"

# External object files for target exception_test
exception_test_EXTERNAL_OBJECTS =

netlib/base/tests/exception_test: netlib/base/tests/CMakeFiles/exception_test.dir/exception_test.cc.o
netlib/base/tests/exception_test: netlib/base/tests/CMakeFiles/exception_test.dir/build.make
netlib/base/tests/exception_test: netlib/base/libnetlib_base.a
netlib/base/tests/exception_test: netlib/base/tests/CMakeFiles/exception_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nwuking/netlib/built/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable exception_test"
	cd /home/nwuking/netlib/built/netlib/base/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exception_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
netlib/base/tests/CMakeFiles/exception_test.dir/build: netlib/base/tests/exception_test

.PHONY : netlib/base/tests/CMakeFiles/exception_test.dir/build

netlib/base/tests/CMakeFiles/exception_test.dir/clean:
	cd /home/nwuking/netlib/built/netlib/base/tests && $(CMAKE_COMMAND) -P CMakeFiles/exception_test.dir/cmake_clean.cmake
.PHONY : netlib/base/tests/CMakeFiles/exception_test.dir/clean

netlib/base/tests/CMakeFiles/exception_test.dir/depend:
	cd /home/nwuking/netlib/built && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nwuking/netlib /home/nwuking/netlib/netlib/base/tests /home/nwuking/netlib/built /home/nwuking/netlib/built/netlib/base/tests /home/nwuking/netlib/built/netlib/base/tests/CMakeFiles/exception_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : netlib/base/tests/CMakeFiles/exception_test.dir/depend

