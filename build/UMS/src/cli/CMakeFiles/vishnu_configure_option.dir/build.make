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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /opt/software/GIT/vishnu_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/software/GIT/vishnu_1/build

# Include any dependencies generated for this target.
include UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/depend.make

# Include the progress variables for this target.
include UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/progress.make

# Include the compile flags for this target's objects.
include UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/flags.make

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/flags.make
UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o: ../UMS/src/cli/configure_option.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/software/GIT/vishnu_1/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o"
	cd /opt/software/GIT/vishnu_1/build/UMS/src/cli && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o -c /opt/software/GIT/vishnu_1/UMS/src/cli/configure_option.cpp

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.i"
	cd /opt/software/GIT/vishnu_1/build/UMS/src/cli && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /opt/software/GIT/vishnu_1/UMS/src/cli/configure_option.cpp > CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.i

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.s"
	cd /opt/software/GIT/vishnu_1/build/UMS/src/cli && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /opt/software/GIT/vishnu_1/UMS/src/cli/configure_option.cpp -o CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.s

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.requires:
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.requires

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.provides: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.requires
	$(MAKE) -f UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/build.make UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.provides.build
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.provides

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.provides.build: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o

# Object files for target vishnu_configure_option
vishnu_configure_option_OBJECTS = \
"CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o"

# External object files for target vishnu_configure_option
vishnu_configure_option_EXTERNAL_OBJECTS =

bin/vishnu_configure_option: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o
bin/vishnu_configure_option: /opt/install/lib/libLogForwarderUtils.so
bin/vishnu_configure_option: /usr/lib/libboost_program_options-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_date_time-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_thread-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_filesystem-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_system-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_unit_test_framework-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_serialization-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_random-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_regex-mt.so
bin/vishnu_configure_option: lib/libvishnu-ums-client.so.1.4.0
bin/vishnu_configure_option: /usr/local/lib/libomniDynamic4.so
bin/vishnu_configure_option: /usr/local/lib/libomniORB4.so
bin/vishnu_configure_option: /usr/local/lib/libomnithread.so
bin/vishnu_configure_option: /usr/local/lib/libCOS4.so
bin/vishnu_configure_option: /usr/local/lib/libCOSDynamic4.so
bin/vishnu_configure_option: /usr/local/lib/libomniDynamic4.so
bin/vishnu_configure_option: /usr/local/lib/libomniORB4.so
bin/vishnu_configure_option: /usr/local/lib/libomnithread.so
bin/vishnu_configure_option: /usr/local/lib/libCOS4.so
bin/vishnu_configure_option: /usr/local/lib/libCOSDynamic4.so
bin/vishnu_configure_option: lib/libvishnu-core.so.1.4.0
bin/vishnu_configure_option: lib/libemf4cpp-vishnu.so.1.4.0
bin/vishnu_configure_option: /usr/lib/x86_64-linux-gnu/libcrypt.so
bin/vishnu_configure_option: /usr/lib/libboost_program_options-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_date_time-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_thread-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_filesystem-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_system-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_unit_test_framework-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_serialization-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_random-mt.so
bin/vishnu_configure_option: /usr/lib/libboost_regex-mt.so
bin/vishnu_configure_option: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/build.make
bin/vishnu_configure_option: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/vishnu_configure_option"
	cd /opt/software/GIT/vishnu_1/build/UMS/src/cli && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vishnu_configure_option.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/build: bin/vishnu_configure_option
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/build

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/requires: UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/configure_option.cpp.o.requires
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/requires

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/clean:
	cd /opt/software/GIT/vishnu_1/build/UMS/src/cli && $(CMAKE_COMMAND) -P CMakeFiles/vishnu_configure_option.dir/cmake_clean.cmake
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/clean

UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/depend:
	cd /opt/software/GIT/vishnu_1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/software/GIT/vishnu_1 /opt/software/GIT/vishnu_1/UMS/src/cli /opt/software/GIT/vishnu_1/build /opt/software/GIT/vishnu_1/build/UMS/src/cli /opt/software/GIT/vishnu_1/build/UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : UMS/src/cli/CMakeFiles/vishnu_configure_option.dir/depend

