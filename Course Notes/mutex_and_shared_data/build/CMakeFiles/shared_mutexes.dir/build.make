# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jeff/Desktop/mutex_and_shared_data

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jeff/Desktop/mutex_and_shared_data/build

# Include any dependencies generated for this target.
include CMakeFiles/shared_mutexes.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/shared_mutexes.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/shared_mutexes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shared_mutexes.dir/flags.make

CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o: CMakeFiles/shared_mutexes.dir/flags.make
CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o: ../shared_mutexes.cpp
CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o: CMakeFiles/shared_mutexes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o -MF CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o.d -o CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o -c /Users/jeff/Desktop/mutex_and_shared_data/shared_mutexes.cpp

CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jeff/Desktop/mutex_and_shared_data/shared_mutexes.cpp > CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.i

CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jeff/Desktop/mutex_and_shared_data/shared_mutexes.cpp -o CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.s

# Object files for target shared_mutexes
shared_mutexes_OBJECTS = \
"CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o"

# External object files for target shared_mutexes
shared_mutexes_EXTERNAL_OBJECTS =

shared_mutexes: CMakeFiles/shared_mutexes.dir/shared_mutexes.cpp.o
shared_mutexes: CMakeFiles/shared_mutexes.dir/build.make
shared_mutexes: CMakeFiles/shared_mutexes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable shared_mutexes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared_mutexes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shared_mutexes.dir/build: shared_mutexes
.PHONY : CMakeFiles/shared_mutexes.dir/build

CMakeFiles/shared_mutexes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shared_mutexes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shared_mutexes.dir/clean

CMakeFiles/shared_mutexes.dir/depend:
	cd /Users/jeff/Desktop/mutex_and_shared_data/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jeff/Desktop/mutex_and_shared_data /Users/jeff/Desktop/mutex_and_shared_data /Users/jeff/Desktop/mutex_and_shared_data/build /Users/jeff/Desktop/mutex_and_shared_data/build /Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles/shared_mutexes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shared_mutexes.dir/depend

