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
CMAKE_SOURCE_DIR = /home/sun/Webserver1/Mywebserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sun/Webserver1/Mywebserver/build

# Include any dependencies generated for this target.
include CMakeFiles/out.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/out.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/out.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/out.exe.dir/flags.make

CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o: ../Server/src/EpollTools.cpp
CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o -MF CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o.d -o CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o -c /home/sun/Webserver1/Mywebserver/Server/src/EpollTools.cpp

CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/Server/src/EpollTools.cpp > CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.i

CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/Server/src/EpollTools.cpp -o CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.s

CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o: ../Server/src/HttpProcessRead.cpp
CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o -MF CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o.d -o CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o -c /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessRead.cpp

CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessRead.cpp > CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.i

CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessRead.cpp -o CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.s

CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o: ../Server/src/HttpProcessWrite.cpp
CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o -MF CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o.d -o CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o -c /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessWrite.cpp

CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessWrite.cpp > CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.i

CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/Server/src/HttpProcessWrite.cpp -o CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.s

CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o: ../Server/src/Httpserver.cpp
CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o -MF CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o.d -o CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o -c /home/sun/Webserver1/Mywebserver/Server/src/Httpserver.cpp

CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/Server/src/Httpserver.cpp > CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.i

CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/Server/src/Httpserver.cpp -o CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.s

CMakeFiles/out.exe.dir/Server/src/locker.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/Server/src/locker.cpp.o: ../Server/src/locker.cpp
CMakeFiles/out.exe.dir/Server/src/locker.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/out.exe.dir/Server/src/locker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/Server/src/locker.cpp.o -MF CMakeFiles/out.exe.dir/Server/src/locker.cpp.o.d -o CMakeFiles/out.exe.dir/Server/src/locker.cpp.o -c /home/sun/Webserver1/Mywebserver/Server/src/locker.cpp

CMakeFiles/out.exe.dir/Server/src/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/Server/src/locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/Server/src/locker.cpp > CMakeFiles/out.exe.dir/Server/src/locker.cpp.i

CMakeFiles/out.exe.dir/Server/src/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/Server/src/locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/Server/src/locker.cpp -o CMakeFiles/out.exe.dir/Server/src/locker.cpp.s

CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o: ../LOG/src/LogBuffer.cpp
CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o -MF CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o.d -o CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o -c /home/sun/Webserver1/Mywebserver/LOG/src/LogBuffer.cpp

CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/LOG/src/LogBuffer.cpp > CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.i

CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/LOG/src/LogBuffer.cpp -o CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.s

CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o: ../LOG/src/LogFile.cpp
CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o -MF CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o.d -o CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o -c /home/sun/Webserver1/Mywebserver/LOG/src/LogFile.cpp

CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/LOG/src/LogFile.cpp > CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.i

CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/LOG/src/LogFile.cpp -o CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.s

CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o: ../LOG/src/LogStream.cpp
CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o -MF CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o.d -o CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o -c /home/sun/Webserver1/Mywebserver/LOG/src/LogStream.cpp

CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/LOG/src/LogStream.cpp > CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.i

CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/LOG/src/LogStream.cpp -o CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.s

CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o: ../LOG/src/LogThread.cpp
CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o -MF CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o.d -o CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o -c /home/sun/Webserver1/Mywebserver/LOG/src/LogThread.cpp

CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/LOG/src/LogThread.cpp > CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.i

CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/LOG/src/LogThread.cpp -o CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.s

CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o: ../LOG/src/Logger.cpp
CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o -MF CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o.d -o CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o -c /home/sun/Webserver1/Mywebserver/LOG/src/Logger.cpp

CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/LOG/src/Logger.cpp > CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.i

CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/LOG/src/Logger.cpp -o CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.s

CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o: ../time/src/timePoint.cpp
CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o -MF CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o.d -o CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o -c /home/sun/Webserver1/Mywebserver/time/src/timePoint.cpp

CMakeFiles/out.exe.dir/time/src/timePoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/time/src/timePoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/time/src/timePoint.cpp > CMakeFiles/out.exe.dir/time/src/timePoint.cpp.i

CMakeFiles/out.exe.dir/time/src/timePoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/time/src/timePoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/time/src/timePoint.cpp -o CMakeFiles/out.exe.dir/time/src/timePoint.cpp.s

CMakeFiles/out.exe.dir/time/src/timmer.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/time/src/timmer.cpp.o: ../time/src/timmer.cpp
CMakeFiles/out.exe.dir/time/src/timmer.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/out.exe.dir/time/src/timmer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/time/src/timmer.cpp.o -MF CMakeFiles/out.exe.dir/time/src/timmer.cpp.o.d -o CMakeFiles/out.exe.dir/time/src/timmer.cpp.o -c /home/sun/Webserver1/Mywebserver/time/src/timmer.cpp

CMakeFiles/out.exe.dir/time/src/timmer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/time/src/timmer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/time/src/timmer.cpp > CMakeFiles/out.exe.dir/time/src/timmer.cpp.i

CMakeFiles/out.exe.dir/time/src/timmer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/time/src/timmer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/time/src/timmer.cpp -o CMakeFiles/out.exe.dir/time/src/timmer.cpp.s

CMakeFiles/out.exe.dir/main.cpp.o: CMakeFiles/out.exe.dir/flags.make
CMakeFiles/out.exe.dir/main.cpp.o: ../main.cpp
CMakeFiles/out.exe.dir/main.cpp.o: CMakeFiles/out.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/out.exe.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/out.exe.dir/main.cpp.o -MF CMakeFiles/out.exe.dir/main.cpp.o.d -o CMakeFiles/out.exe.dir/main.cpp.o -c /home/sun/Webserver1/Mywebserver/main.cpp

CMakeFiles/out.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/out.exe.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sun/Webserver1/Mywebserver/main.cpp > CMakeFiles/out.exe.dir/main.cpp.i

CMakeFiles/out.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/out.exe.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sun/Webserver1/Mywebserver/main.cpp -o CMakeFiles/out.exe.dir/main.cpp.s

# Object files for target out.exe
out_exe_OBJECTS = \
"CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o" \
"CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o" \
"CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o" \
"CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o" \
"CMakeFiles/out.exe.dir/Server/src/locker.cpp.o" \
"CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o" \
"CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o" \
"CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o" \
"CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o" \
"CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o" \
"CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o" \
"CMakeFiles/out.exe.dir/time/src/timmer.cpp.o" \
"CMakeFiles/out.exe.dir/main.cpp.o"

# External object files for target out.exe
out_exe_EXTERNAL_OBJECTS =

out.exe: CMakeFiles/out.exe.dir/Server/src/EpollTools.cpp.o
out.exe: CMakeFiles/out.exe.dir/Server/src/HttpProcessRead.cpp.o
out.exe: CMakeFiles/out.exe.dir/Server/src/HttpProcessWrite.cpp.o
out.exe: CMakeFiles/out.exe.dir/Server/src/Httpserver.cpp.o
out.exe: CMakeFiles/out.exe.dir/Server/src/locker.cpp.o
out.exe: CMakeFiles/out.exe.dir/LOG/src/LogBuffer.cpp.o
out.exe: CMakeFiles/out.exe.dir/LOG/src/LogFile.cpp.o
out.exe: CMakeFiles/out.exe.dir/LOG/src/LogStream.cpp.o
out.exe: CMakeFiles/out.exe.dir/LOG/src/LogThread.cpp.o
out.exe: CMakeFiles/out.exe.dir/LOG/src/Logger.cpp.o
out.exe: CMakeFiles/out.exe.dir/time/src/timePoint.cpp.o
out.exe: CMakeFiles/out.exe.dir/time/src/timmer.cpp.o
out.exe: CMakeFiles/out.exe.dir/main.cpp.o
out.exe: CMakeFiles/out.exe.dir/build.make
out.exe: CMakeFiles/out.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sun/Webserver1/Mywebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable out.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/out.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/out.exe.dir/build: out.exe
.PHONY : CMakeFiles/out.exe.dir/build

CMakeFiles/out.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/out.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/out.exe.dir/clean

CMakeFiles/out.exe.dir/depend:
	cd /home/sun/Webserver1/Mywebserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sun/Webserver1/Mywebserver /home/sun/Webserver1/Mywebserver /home/sun/Webserver1/Mywebserver/build /home/sun/Webserver1/Mywebserver/build /home/sun/Webserver1/Mywebserver/build/CMakeFiles/out.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/out.exe.dir/depend

