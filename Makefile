# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:/Program Files/CMake/bin/cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:/Program Files/CMake/bin/cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:/Program Files/CMake/bin/cmake.exe" -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:/Program Files/CMake/bin/cmake.exe" -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"C:/Program Files/CMake/bin/cmake-gui.exe" -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"C:/Program Files/CMake/bin/cmake.exe" --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(CMAKE_COMMAND) -E cmake_progress_start C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/CMakeFiles C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/CDNNExampleApplication//CMakeFiles/progress.marks
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CDNNExampleApplication/all
	$(CMAKE_COMMAND) -E cmake_progress_start C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CDNNExampleApplication/clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CDNNExampleApplication/preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CDNNExampleApplication/preinstall
.PHONY : preinstall/fast

# Convenience name for target.
CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/rule:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/rule
.PHONY : CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/rule

# Convenience name for target.
CDNNExampleApplication: CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/rule

.PHONY : CDNNExampleApplication

# fast build rule for target.
CDNNExampleApplication/fast:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build
.PHONY : CDNNExampleApplication/fast

src/ProcessAuxiliary.o: src/ProcessAuxiliary.c.o

.PHONY : src/ProcessAuxiliary.o

# target to build an object file
src/ProcessAuxiliary.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/ProcessAuxiliary.c.o
.PHONY : src/ProcessAuxiliary.c.o

src/ProcessAuxiliary.i: src/ProcessAuxiliary.c.i

.PHONY : src/ProcessAuxiliary.i

# target to preprocess a source file
src/ProcessAuxiliary.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/ProcessAuxiliary.c.i
.PHONY : src/ProcessAuxiliary.c.i

src/ProcessAuxiliary.s: src/ProcessAuxiliary.c.s

.PHONY : src/ProcessAuxiliary.s

# target to generate assembly for a file
src/ProcessAuxiliary.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/ProcessAuxiliary.c.s
.PHONY : src/ProcessAuxiliary.c.s

src/inference.o: src/inference.c.o

.PHONY : src/inference.o

# target to build an object file
src/inference.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/inference.c.o
.PHONY : src/inference.c.o

src/inference.i: src/inference.c.i

.PHONY : src/inference.i

# target to preprocess a source file
src/inference.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/inference.c.i
.PHONY : src/inference.c.i

src/inference.s: src/inference.c.s

.PHONY : src/inference.s

# target to generate assembly for a file
src/inference.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/inference.c.s
.PHONY : src/inference.c.s

src/mainApplication.o: src/mainApplication.c.o

.PHONY : src/mainApplication.o

# target to build an object file
src/mainApplication.c.o:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/mainApplication.c.o
.PHONY : src/mainApplication.c.o

src/mainApplication.i: src/mainApplication.c.i

.PHONY : src/mainApplication.i

# target to preprocess a source file
src/mainApplication.c.i:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/mainApplication.c.i
.PHONY : src/mainApplication.c.i

src/mainApplication.s: src/mainApplication.c.s

.PHONY : src/mainApplication.s

# target to generate assembly for a file
src/mainApplication.c.s:
	cd C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNN/main && $(MAKE) $(MAKESILENT) -f CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/build.make CDNNExampleApplication/CMakeFiles/CDNNExampleApplication.dir/src/mainApplication.c.s
.PHONY : src/mainApplication.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... CDNNExampleApplication"
	@echo "... src/ProcessAuxiliary.o"
	@echo "... src/ProcessAuxiliary.i"
	@echo "... src/ProcessAuxiliary.s"
	@echo "... src/inference.o"
	@echo "... src/inference.i"
	@echo "... src/inference.s"
	@echo "... src/mainApplication.o"
	@echo "... src/mainApplication.i"
	@echo "... src/mainApplication.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.
