# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_SOURCE_DIR = /root/socketli

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/socketli/build

# Include any dependencies generated for this target.
include server/CMakeFiles/CHttpServer.dir/depend.make

# Include the progress variables for this target.
include server/CMakeFiles/CHttpServer.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/CHttpServer.dir/flags.make

server/CMakeFiles/CHttpServer.dir/main.cc.o: server/CMakeFiles/CHttpServer.dir/flags.make
server/CMakeFiles/CHttpServer.dir/main.cc.o: ../server/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/socketli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/CHttpServer.dir/main.cc.o"
	cd /root/socketli/build/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CHttpServer.dir/main.cc.o -c /root/socketli/server/main.cc

server/CMakeFiles/CHttpServer.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CHttpServer.dir/main.cc.i"
	cd /root/socketli/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/socketli/server/main.cc > CMakeFiles/CHttpServer.dir/main.cc.i

server/CMakeFiles/CHttpServer.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CHttpServer.dir/main.cc.s"
	cd /root/socketli/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/socketli/server/main.cc -o CMakeFiles/CHttpServer.dir/main.cc.s

# Object files for target CHttpServer
CHttpServer_OBJECTS = \
"CMakeFiles/CHttpServer.dir/main.cc.o"

# External object files for target CHttpServer
CHttpServer_EXTERNAL_OBJECTS =

server/CHttpServer: server/CMakeFiles/CHttpServer.dir/main.cc.o
server/CHttpServer: server/CMakeFiles/CHttpServer.dir/build.make
server/CHttpServer: server/CMakeFiles/CHttpServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/socketli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CHttpServer"
	cd /root/socketli/build/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CHttpServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/CHttpServer.dir/build: server/CHttpServer

.PHONY : server/CMakeFiles/CHttpServer.dir/build

server/CMakeFiles/CHttpServer.dir/clean:
	cd /root/socketli/build/server && $(CMAKE_COMMAND) -P CMakeFiles/CHttpServer.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/CHttpServer.dir/clean

server/CMakeFiles/CHttpServer.dir/depend:
	cd /root/socketli/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/socketli /root/socketli/server /root/socketli/build /root/socketli/build/server /root/socketli/build/server/CMakeFiles/CHttpServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/CHttpServer.dir/depend
