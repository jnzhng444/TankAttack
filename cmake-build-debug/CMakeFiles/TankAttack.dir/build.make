# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /snap/clion/296/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/296/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/brema026/Documentos/GitHub/TankAttack

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TankAttack.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TankAttack.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TankAttack.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TankAttack.dir/flags.make

CMakeFiles/TankAttack.dir/main.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/main.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/main.cpp
CMakeFiles/TankAttack.dir/main.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TankAttack.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/main.cpp.o -MF CMakeFiles/TankAttack.dir/main.cpp.o.d -o CMakeFiles/TankAttack.dir/main.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/main.cpp

CMakeFiles/TankAttack.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/main.cpp > CMakeFiles/TankAttack.dir/main.cpp.i

CMakeFiles/TankAttack.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/main.cpp -o CMakeFiles/TankAttack.dir/main.cpp.s

CMakeFiles/TankAttack.dir/GameWindow.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/GameWindow.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/GameWindow.cpp
CMakeFiles/TankAttack.dir/GameWindow.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TankAttack.dir/GameWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/GameWindow.cpp.o -MF CMakeFiles/TankAttack.dir/GameWindow.cpp.o.d -o CMakeFiles/TankAttack.dir/GameWindow.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/GameWindow.cpp

CMakeFiles/TankAttack.dir/GameWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/GameWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/GameWindow.cpp > CMakeFiles/TankAttack.dir/GameWindow.cpp.i

CMakeFiles/TankAttack.dir/GameWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/GameWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/GameWindow.cpp -o CMakeFiles/TankAttack.dir/GameWindow.cpp.s

CMakeFiles/TankAttack.dir/GameArea.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/GameArea.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/GameArea.cpp
CMakeFiles/TankAttack.dir/GameArea.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TankAttack.dir/GameArea.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/GameArea.cpp.o -MF CMakeFiles/TankAttack.dir/GameArea.cpp.o.d -o CMakeFiles/TankAttack.dir/GameArea.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/GameArea.cpp

CMakeFiles/TankAttack.dir/GameArea.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/GameArea.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/GameArea.cpp > CMakeFiles/TankAttack.dir/GameArea.cpp.i

CMakeFiles/TankAttack.dir/GameArea.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/GameArea.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/GameArea.cpp -o CMakeFiles/TankAttack.dir/GameArea.cpp.s

CMakeFiles/TankAttack.dir/GameLogic.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/GameLogic.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/GameLogic.cpp
CMakeFiles/TankAttack.dir/GameLogic.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TankAttack.dir/GameLogic.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/GameLogic.cpp.o -MF CMakeFiles/TankAttack.dir/GameLogic.cpp.o.d -o CMakeFiles/TankAttack.dir/GameLogic.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/GameLogic.cpp

CMakeFiles/TankAttack.dir/GameLogic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/GameLogic.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/GameLogic.cpp > CMakeFiles/TankAttack.dir/GameLogic.cpp.i

CMakeFiles/TankAttack.dir/GameLogic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/GameLogic.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/GameLogic.cpp -o CMakeFiles/TankAttack.dir/GameLogic.cpp.s

CMakeFiles/TankAttack.dir/Map.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/Map.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/Map.cpp
CMakeFiles/TankAttack.dir/Map.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TankAttack.dir/Map.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/Map.cpp.o -MF CMakeFiles/TankAttack.dir/Map.cpp.o.d -o CMakeFiles/TankAttack.dir/Map.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/Map.cpp

CMakeFiles/TankAttack.dir/Map.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/Map.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/Map.cpp > CMakeFiles/TankAttack.dir/Map.cpp.i

CMakeFiles/TankAttack.dir/Map.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/Map.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/Map.cpp -o CMakeFiles/TankAttack.dir/Map.cpp.s

CMakeFiles/TankAttack.dir/Pathfinding.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/Pathfinding.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/Pathfinding.cpp
CMakeFiles/TankAttack.dir/Pathfinding.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TankAttack.dir/Pathfinding.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/Pathfinding.cpp.o -MF CMakeFiles/TankAttack.dir/Pathfinding.cpp.o.d -o CMakeFiles/TankAttack.dir/Pathfinding.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/Pathfinding.cpp

CMakeFiles/TankAttack.dir/Pathfinding.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/Pathfinding.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/Pathfinding.cpp > CMakeFiles/TankAttack.dir/Pathfinding.cpp.i

CMakeFiles/TankAttack.dir/Pathfinding.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/Pathfinding.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/Pathfinding.cpp -o CMakeFiles/TankAttack.dir/Pathfinding.cpp.s

CMakeFiles/TankAttack.dir/Projectile.cpp.o: CMakeFiles/TankAttack.dir/flags.make
CMakeFiles/TankAttack.dir/Projectile.cpp.o: /home/brema026/Documentos/GitHub/TankAttack/Projectile.cpp
CMakeFiles/TankAttack.dir/Projectile.cpp.o: CMakeFiles/TankAttack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TankAttack.dir/Projectile.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TankAttack.dir/Projectile.cpp.o -MF CMakeFiles/TankAttack.dir/Projectile.cpp.o.d -o CMakeFiles/TankAttack.dir/Projectile.cpp.o -c /home/brema026/Documentos/GitHub/TankAttack/Projectile.cpp

CMakeFiles/TankAttack.dir/Projectile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TankAttack.dir/Projectile.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/brema026/Documentos/GitHub/TankAttack/Projectile.cpp > CMakeFiles/TankAttack.dir/Projectile.cpp.i

CMakeFiles/TankAttack.dir/Projectile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TankAttack.dir/Projectile.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/brema026/Documentos/GitHub/TankAttack/Projectile.cpp -o CMakeFiles/TankAttack.dir/Projectile.cpp.s

# Object files for target TankAttack
TankAttack_OBJECTS = \
"CMakeFiles/TankAttack.dir/main.cpp.o" \
"CMakeFiles/TankAttack.dir/GameWindow.cpp.o" \
"CMakeFiles/TankAttack.dir/GameArea.cpp.o" \
"CMakeFiles/TankAttack.dir/GameLogic.cpp.o" \
"CMakeFiles/TankAttack.dir/Map.cpp.o" \
"CMakeFiles/TankAttack.dir/Pathfinding.cpp.o" \
"CMakeFiles/TankAttack.dir/Projectile.cpp.o"

# External object files for target TankAttack
TankAttack_EXTERNAL_OBJECTS =

TankAttack: CMakeFiles/TankAttack.dir/main.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/GameWindow.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/GameArea.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/GameLogic.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/Map.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/Pathfinding.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/Projectile.cpp.o
TankAttack: CMakeFiles/TankAttack.dir/build.make
TankAttack: CMakeFiles/TankAttack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable TankAttack"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TankAttack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TankAttack.dir/build: TankAttack
.PHONY : CMakeFiles/TankAttack.dir/build

CMakeFiles/TankAttack.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TankAttack.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TankAttack.dir/clean

CMakeFiles/TankAttack.dir/depend:
	cd /home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/brema026/Documentos/GitHub/TankAttack /home/brema026/Documentos/GitHub/TankAttack /home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug /home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug /home/brema026/Documentos/GitHub/TankAttack/cmake-build-debug/CMakeFiles/TankAttack.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TankAttack.dir/depend

