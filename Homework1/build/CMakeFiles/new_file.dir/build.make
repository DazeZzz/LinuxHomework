# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /root/Homework1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Homework1/build

# Include any dependencies generated for this target.
include CMakeFiles/new_file.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/new_file.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/new_file.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/new_file.dir/flags.make

CMakeFiles/new_file.dir/new_file.cpp.o: CMakeFiles/new_file.dir/flags.make
CMakeFiles/new_file.dir/new_file.cpp.o: ../new_file.cpp
CMakeFiles/new_file.dir/new_file.cpp.o: CMakeFiles/new_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Homework1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/new_file.dir/new_file.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/new_file.dir/new_file.cpp.o -MF CMakeFiles/new_file.dir/new_file.cpp.o.d -o CMakeFiles/new_file.dir/new_file.cpp.o -c /root/Homework1/new_file.cpp

CMakeFiles/new_file.dir/new_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/new_file.dir/new_file.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Homework1/new_file.cpp > CMakeFiles/new_file.dir/new_file.cpp.i

CMakeFiles/new_file.dir/new_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/new_file.dir/new_file.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Homework1/new_file.cpp -o CMakeFiles/new_file.dir/new_file.cpp.s

# Object files for target new_file
new_file_OBJECTS = \
"CMakeFiles/new_file.dir/new_file.cpp.o"

# External object files for target new_file
new_file_EXTERNAL_OBJECTS =

libnew_file.a: CMakeFiles/new_file.dir/new_file.cpp.o
libnew_file.a: CMakeFiles/new_file.dir/build.make
libnew_file.a: CMakeFiles/new_file.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Homework1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libnew_file.a"
	$(CMAKE_COMMAND) -P CMakeFiles/new_file.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/new_file.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/new_file.dir/build: libnew_file.a
.PHONY : CMakeFiles/new_file.dir/build

CMakeFiles/new_file.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/new_file.dir/cmake_clean.cmake
.PHONY : CMakeFiles/new_file.dir/clean

CMakeFiles/new_file.dir/depend:
	cd /root/Homework1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Homework1 /root/Homework1 /root/Homework1/build /root/Homework1/build /root/Homework1/build/CMakeFiles/new_file.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/new_file.dir/depend

