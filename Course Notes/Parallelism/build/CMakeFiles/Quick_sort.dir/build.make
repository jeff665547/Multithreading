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
CMAKE_SOURCE_DIR = /Users/jeff/Desktop/Parallelism

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jeff/Desktop/Parallelism/build

# Include any dependencies generated for this target.
include CMakeFiles/Quick_sort.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Quick_sort.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Quick_sort.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Quick_sort.dir/flags.make

CMakeFiles/Quick_sort.dir/quick_sort.cpp.o: CMakeFiles/Quick_sort.dir/flags.make
CMakeFiles/Quick_sort.dir/quick_sort.cpp.o: ../quick_sort.cpp
CMakeFiles/Quick_sort.dir/quick_sort.cpp.o: CMakeFiles/Quick_sort.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jeff/Desktop/Parallelism/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Quick_sort.dir/quick_sort.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Quick_sort.dir/quick_sort.cpp.o -MF CMakeFiles/Quick_sort.dir/quick_sort.cpp.o.d -o CMakeFiles/Quick_sort.dir/quick_sort.cpp.o -c /Users/jeff/Desktop/Parallelism/quick_sort.cpp

CMakeFiles/Quick_sort.dir/quick_sort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Quick_sort.dir/quick_sort.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jeff/Desktop/Parallelism/quick_sort.cpp > CMakeFiles/Quick_sort.dir/quick_sort.cpp.i

CMakeFiles/Quick_sort.dir/quick_sort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Quick_sort.dir/quick_sort.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jeff/Desktop/Parallelism/quick_sort.cpp -o CMakeFiles/Quick_sort.dir/quick_sort.cpp.s

# Object files for target Quick_sort
Quick_sort_OBJECTS = \
"CMakeFiles/Quick_sort.dir/quick_sort.cpp.o"

# External object files for target Quick_sort
Quick_sort_EXTERNAL_OBJECTS =

Quick_sort: CMakeFiles/Quick_sort.dir/quick_sort.cpp.o
Quick_sort: CMakeFiles/Quick_sort.dir/build.make
Quick_sort: CMakeFiles/Quick_sort.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jeff/Desktop/Parallelism/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Quick_sort"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Quick_sort.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Quick_sort.dir/build: Quick_sort
.PHONY : CMakeFiles/Quick_sort.dir/build

CMakeFiles/Quick_sort.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Quick_sort.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Quick_sort.dir/clean

CMakeFiles/Quick_sort.dir/depend:
	cd /Users/jeff/Desktop/Parallelism/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jeff/Desktop/Parallelism /Users/jeff/Desktop/Parallelism /Users/jeff/Desktop/Parallelism/build /Users/jeff/Desktop/Parallelism/build /Users/jeff/Desktop/Parallelism/build/CMakeFiles/Quick_sort.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Quick_sort.dir/depend

