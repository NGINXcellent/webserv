# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build

# Include any dependencies generated for this target.
include HttpTests/CMakeFiles/HttpTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include HttpTests/CMakeFiles/HttpTests.dir/compiler_depend.make

# Include the progress variables for this target.
include HttpTests/CMakeFiles/HttpTests.dir/progress.make

# Include the compile flags for this target's objects.
include HttpTests/CMakeFiles/HttpTests.dir/flags.make

HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o: HttpTests/CMakeFiles/HttpTests.dir/flags.make
HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o: /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/HttpStatusTests.cpp
HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o: HttpTests/CMakeFiles/HttpTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o -MF CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o.d -o CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o -c /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/HttpStatusTests.cpp

HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.i"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/HttpStatusTests.cpp > CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.i

HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.s"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/HttpStatusTests.cpp -o CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.s

HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o: HttpTests/CMakeFiles/HttpTests.dir/flags.make
HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o: /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/ServerTests.cpp
HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o: HttpTests/CMakeFiles/HttpTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o -MF CMakeFiles/HttpTests.dir/ServerTests.cpp.o.d -o CMakeFiles/HttpTests.dir/ServerTests.cpp.o -c /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/ServerTests.cpp

HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HttpTests.dir/ServerTests.cpp.i"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/ServerTests.cpp > CMakeFiles/HttpTests.dir/ServerTests.cpp.i

HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HttpTests.dir/ServerTests.cpp.s"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests/ServerTests.cpp -o CMakeFiles/HttpTests.dir/ServerTests.cpp.s

# Object files for target HttpTests
HttpTests_OBJECTS = \
"CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o" \
"CMakeFiles/HttpTests.dir/ServerTests.cpp.o"

# External object files for target HttpTests
HttpTests_EXTERNAL_OBJECTS =

HttpTests/HttpTests: HttpTests/CMakeFiles/HttpTests.dir/HttpStatusTests.cpp.o
HttpTests/HttpTests: HttpTests/CMakeFiles/HttpTests.dir/ServerTests.cpp.o
HttpTests/HttpTests: HttpTests/CMakeFiles/HttpTests.dir/build.make
HttpTests/HttpTests: lib/libgtest_main.a
HttpTests/HttpTests: libwebserv.a
HttpTests/HttpTests: lib/libgtest.a
HttpTests/HttpTests: HttpTests/CMakeFiles/HttpTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable HttpTests"
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HttpTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
HttpTests/CMakeFiles/HttpTests.dir/build: HttpTests/HttpTests
.PHONY : HttpTests/CMakeFiles/HttpTests.dir/build

HttpTests/CMakeFiles/HttpTests.dir/clean:
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests && $(CMAKE_COMMAND) -P CMakeFiles/HttpTests.dir/cmake_clean.cmake
.PHONY : HttpTests/CMakeFiles/HttpTests.dir/clean

HttpTests/CMakeFiles/HttpTests.dir/depend:
	cd /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/HttpTests /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests /home/freemanc14/Projects/42cursus/webserv/tests/unit_tests/build/HttpTests/CMakeFiles/HttpTests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : HttpTests/CMakeFiles/HttpTests.dir/depend

