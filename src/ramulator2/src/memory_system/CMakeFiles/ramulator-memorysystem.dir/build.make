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
CMAKE_SOURCE_DIR = /home/ksgoura/os_aware_memory_controller/scarab/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ksgoura/os_aware_memory_controller/scarab/src

# Include any dependencies generated for this target.
include ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/compiler_depend.make

# Include the progress variables for this target.
include ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/progress.make

# Include the compile flags for this target's objects.
include ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/flags.make

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/flags.make
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o: ramulator2/src/memory_system/impl/bh_DRAM_system.cpp
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ksgoura/os_aware_memory_controller/scarab/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o -MF CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o.d -o CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o -c /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/bh_DRAM_system.cpp

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.i"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/bh_DRAM_system.cpp > CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.i

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.s"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/bh_DRAM_system.cpp -o CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.s

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/flags.make
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o: ramulator2/src/memory_system/impl/dummy_memory_system.cpp
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ksgoura/os_aware_memory_controller/scarab/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o -MF CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o.d -o CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o -c /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/dummy_memory_system.cpp

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.i"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/dummy_memory_system.cpp > CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.i

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.s"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/dummy_memory_system.cpp -o CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.s

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/flags.make
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o: ramulator2/src/memory_system/impl/generic_DRAM_system.cpp
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ksgoura/os_aware_memory_controller/scarab/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o -MF CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o.d -o CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o -c /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/generic_DRAM_system.cpp

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.i"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/generic_DRAM_system.cpp > CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.i

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.s"
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/impl/generic_DRAM_system.cpp -o CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.s

ramulator-memorysystem: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/bh_DRAM_system.cpp.o
ramulator-memorysystem: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/dummy_memory_system.cpp.o
ramulator-memorysystem: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/impl/generic_DRAM_system.cpp.o
ramulator-memorysystem: ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/build.make
.PHONY : ramulator-memorysystem

# Rule to build all files generated by this target.
ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/build: ramulator-memorysystem
.PHONY : ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/build

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/clean:
	cd /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system && $(CMAKE_COMMAND) -P CMakeFiles/ramulator-memorysystem.dir/cmake_clean.cmake
.PHONY : ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/clean

ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/depend:
	cd /home/ksgoura/os_aware_memory_controller/scarab/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ksgoura/os_aware_memory_controller/scarab/src /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system /home/ksgoura/os_aware_memory_controller/scarab/src /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system /home/ksgoura/os_aware_memory_controller/scarab/src/ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ramulator2/src/memory_system/CMakeFiles/ramulator-memorysystem.dir/depend

