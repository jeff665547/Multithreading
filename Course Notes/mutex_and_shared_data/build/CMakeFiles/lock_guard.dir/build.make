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
include CMakeFiles/lock_guard.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lock_guard.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lock_guard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lock_guard.dir/flags.make

CMakeFiles/lock_guard.dir/lock_guard.cpp.o: CMakeFiles/lock_guard.dir/flags.make
CMakeFiles/lock_guard.dir/lock_guard.cpp.o: ../lock_guard.cpp
CMakeFiles/lock_guard.dir/lock_guard.cpp.o: CMakeFiles/lock_guard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lock_guard.dir/lock_guard.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lock_guard.dir/lock_guard.cpp.o -MF CMakeFiles/lock_guard.dir/lock_guard.cpp.o.d -o CMakeFiles/lock_guard.dir/lock_guard.cpp.o -c /Users/jeff/Desktop/mutex_and_shared_data/lock_guard.cpp

CMakeFiles/lock_guard.dir/lock_guard.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lock_guard.dir/lock_guard.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jeff/Desktop/mutex_and_shared_data/lock_guard.cpp > CMakeFiles/lock_guard.dir/lock_guard.cpp.i

CMakeFiles/lock_guard.dir/lock_guard.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lock_guard.dir/lock_guard.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jeff/Desktop/mutex_and_shared_data/lock_guard.cpp -o CMakeFiles/lock_guard.dir/lock_guard.cpp.s

# Object files for target lock_guard
lock_guard_OBJECTS = \
"CMakeFiles/lock_guard.dir/lock_guard.cpp.o"

# External object files for target lock_guard
lock_guard_EXTERNAL_OBJECTS =

lock_guard: CMakeFiles/lock_guard.dir/lock_guard.cpp.o
lock_guard: CMakeFiles/lock_guard.dir/build.make
lock_guard: CMakeFiles/lock_guard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lock_guard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lock_guard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lock_guard.dir/build: lock_guard
.PHONY : CMakeFiles/lock_guard.dir/build

CMakeFiles/lock_guard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lock_guard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lock_guard.dir/clean

CMakeFiles/lock_guard.dir/depend:
	cd /Users/jeff/Desktop/mutex_and_shared_data/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jeff/Desktop/mutex_and_shared_data /Users/jeff/Desktop/mutex_and_shared_data /Users/jeff/Desktop/mutex_and_shared_data/build /Users/jeff/Desktop/mutex_and_shared_data/build /Users/jeff/Desktop/mutex_and_shared_data/build/CMakeFiles/lock_guard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lock_guard.dir/depend

