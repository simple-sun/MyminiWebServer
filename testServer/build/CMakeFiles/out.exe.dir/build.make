# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_SOURCE_DIR = /home/sun/Webserver1/testServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sun/Webserver1/testServer/build

# Include any dependencies generated for this target.
include CMakeFiles/out.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/out.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/out.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/out.exe.dir/flags.make

CMakeFiles/out.exe.dir/MyTools.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/MyTools.cpp.o: ../MyTools.cpp
CMakeFiles/out.exe.dir/MyTools.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/testServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/out.exe.dir/MyTools.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/MyTools.cpp.o -MF CMakeFiles/out.exe.dir/MyTools.cpp.o.d -o CMakeFiles/out.exe.dir/MyTools.cpp.o -c /home/sun/Webserver1/testServer/MyTools.cpp

CMakeFiles/out.exe.dir/MyTools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/MyTools.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/testServer/MyTools.cpp > CMakeFiles/out.exe.dir/MyTools.cpp.i

CMakeFiles/out.exe.dir/MyTools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/MyTools.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/testServer/MyTools.cpp -o CMakeFiles/out.exe.dir/MyTools.cpp.s

CMakeFiles/out.exe.dir/main.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/main.cpp.o: ../main.cpp
CMakeFiles/out.exe.dir/main.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/testServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/out.exe.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/main.cpp.o -MF CMakeFiles/out.exe.dir/main.cpp.o.d -o CMakeFiles/out.exe.dir/main.cpp.o -c /home/sun/Webserver1/testServer/main.cpp

CMakeFiles/out.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/testServer/main.cpp > CMakeFiles/out.exe.dir/main.cpp.i

CMakeFiles/out.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/testServer/main.cpp -o CMakeFiles/out.exe.dir/main.cpp.s

# Object files for target out.exe
out_exe_OBJECTS = \
"CMakeFiles/out.exe.dir/MyTools.cpp.o" \
"CMakeFiles/out.exe.dir/main.cpp.o"

# External object files for target out.exe
out_exe_EXTERNAL_OBJECTS =

out.exe: CMakeFiles/out.exe.dir/MyTools.cpp.o
out.exe: CMakeFiles/out.exe.dir/main.cpp.o
out.exe: CMakeFiles/out.exe.dir/build.make
out.exe: CMakeFiles/out.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sun/Webserver1/testServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable out.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/out.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/out.exe.dir/build: out.exe
.PHONY : CMakeFiles/out.exe.dir/build

CMakeFiles/out.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/out.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/out.exe.dir/clean

CMakeFiles/out.exe.dir/depend:
	cd /home/sun/Webserver1/testServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sun/Webserver1/testServer /home/sun/Webserver1/testServer /home/sun/Webserver1/testServer/build /home/sun/Webserver1/testServer/build /home/sun/Webserver1/testServer/build/CMakeFiles/out.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/out.exe.dir/depend

