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
CMAKE_SOURCE_DIR = /Users/jeff/Desktop/Atomic_Types

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jeff/Desktop/Atomic_Types/build

# Include any dependencies generated for this target.
include CMakeFiles/Integer_operations_and_threads.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Integer_operations_and_threads.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Integer_operations_and_threads.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Integer_operations_and_threads.dir/flags.make

CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o: CMakeFiles/Integer_operations_and_threads.dir/flags.make
CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o: ../Integer_operations_and_threads.cpp
CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o: CMakeFiles/Integer_operations_and_threads.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jeff/Desktop/Atomic_Types/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o -MF CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o.d -o CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o -c /Users/jeff/Desktop/Atomic_Types/Integer_operations_and_threads.cpp

CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jeff/Desktop/Atomic_Types/Integer_operations_and_threads.cpp > CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.i

CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jeff/Desktop/Atomic_Types/Integer_operations_and_threads.cpp -o CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.s

# Object files for target Integer_operations_and_threads
Integer_operations_and_threads_OBJECTS = \
"CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o"

# External object files for target Integer_operations_and_threads
Integer_operations_and_threads_EXTERNAL_OBJECTS =

Integer_operations_and_threads: CMakeFiles/Integer_operations_and_threads.dir/Integer_operations_and_threads.cpp.o
Integer_operations_and_threads: CMakeFiles/Integer_operations_and_threads.dir/build.make
Integer_operations_and_threads: CMakeFiles/Integer_operations_and_threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jeff/Desktop/Atomic_Types/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Integer_operations_and_threads"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Integer_operations_and_threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Integer_operations_and_threads.dir/build: Integer_operations_and_threads
.PHONY : CMakeFiles/Integer_operations_and_threads.dir/build

CMakeFiles/Integer_operations_and_threads.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Integer_operations_and_threads.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Integer_operations_and_threads.dir/clean

CMakeFiles/Integer_operations_and_threads.dir/depend:
	cd /Users/jeff/Desktop/Atomic_Types/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jeff/Desktop/Atomic_Types /Users/jeff/Desktop/Atomic_Types /Users/jeff/Desktop/Atomic_Types/build /Users/jeff/Desktop/Atomic_Types/build /Users/jeff/Desktop/Atomic_Types/build/CMakeFiles/Integer_operations_and_threads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Integer_operations_and_threads.dir/depend
