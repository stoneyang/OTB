# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = "/Applications/CMake 2.8-0.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-0.app/Contents/bin/cmake" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-0.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/liblas-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/liblas-main

# Include any dependencies generated for this target.
include apps/CMakeFiles/lasinfo-old.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/lasinfo-old.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/lasinfo-old.dir/flags.make

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o: apps/CMakeFiles/lasinfo-old.dir/flags.make
apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o: apps/lascommon.c
	$(CMAKE_COMMAND) -E cmake_progress_report /tmp/liblas-main/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lasinfo-old.dir/lascommon.c.o   -c /tmp/liblas-main/apps/lascommon.c

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lasinfo-old.dir/lascommon.c.i"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /tmp/liblas-main/apps/lascommon.c > CMakeFiles/lasinfo-old.dir/lascommon.c.i

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lasinfo-old.dir/lascommon.c.s"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /tmp/liblas-main/apps/lascommon.c -o CMakeFiles/lasinfo-old.dir/lascommon.c.s

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.requires:
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.requires

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.provides: apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.requires
	$(MAKE) -f apps/CMakeFiles/lasinfo-old.dir/build.make apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.provides.build
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.provides

apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.provides.build: apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.provides.build

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o: apps/CMakeFiles/lasinfo-old.dir/flags.make
apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o: apps/lasinfo-old.c
	$(CMAKE_COMMAND) -E cmake_progress_report /tmp/liblas-main/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o   -c /tmp/liblas-main/apps/lasinfo-old.c

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lasinfo-old.dir/lasinfo-old.c.i"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /tmp/liblas-main/apps/lasinfo-old.c > CMakeFiles/lasinfo-old.dir/lasinfo-old.c.i

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lasinfo-old.dir/lasinfo-old.c.s"
	cd /tmp/liblas-main/apps && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /tmp/liblas-main/apps/lasinfo-old.c -o CMakeFiles/lasinfo-old.dir/lasinfo-old.c.s

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.requires:
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.requires

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.provides: apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.requires
	$(MAKE) -f apps/CMakeFiles/lasinfo-old.dir/build.make apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.provides.build
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.provides

apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.provides.build: apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o
.PHONY : apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.provides.build

# Object files for target lasinfo-old
lasinfo__old_OBJECTS = \
"CMakeFiles/lasinfo-old.dir/lascommon.c.o" \
"CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o"

# External object files for target lasinfo-old
lasinfo__old_EXTERNAL_OBJECTS =

bin/lasinfo-old: apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o
bin/lasinfo-old: apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o
bin/lasinfo-old: bin/liblas_c.dylib
bin/lasinfo-old: bin/liblas.a
bin/lasinfo-old: apps/CMakeFiles/lasinfo-old.dir/build.make
bin/lasinfo-old: apps/CMakeFiles/lasinfo-old.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/lasinfo-old"
	cd /tmp/liblas-main/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lasinfo-old.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/lasinfo-old.dir/build: bin/lasinfo-old
.PHONY : apps/CMakeFiles/lasinfo-old.dir/build

apps/CMakeFiles/lasinfo-old.dir/requires: apps/CMakeFiles/lasinfo-old.dir/lascommon.c.o.requires
apps/CMakeFiles/lasinfo-old.dir/requires: apps/CMakeFiles/lasinfo-old.dir/lasinfo-old.c.o.requires
.PHONY : apps/CMakeFiles/lasinfo-old.dir/requires

apps/CMakeFiles/lasinfo-old.dir/clean:
	cd /tmp/liblas-main/apps && $(CMAKE_COMMAND) -P CMakeFiles/lasinfo-old.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/lasinfo-old.dir/clean

apps/CMakeFiles/lasinfo-old.dir/depend:
	cd /tmp/liblas-main && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/liblas-main /tmp/liblas-main/apps /tmp/liblas-main /tmp/liblas-main/apps /tmp/liblas-main/apps/CMakeFiles/lasinfo-old.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/lasinfo-old.dir/depend

