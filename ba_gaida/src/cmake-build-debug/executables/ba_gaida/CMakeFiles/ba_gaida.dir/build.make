# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Dradozer\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\191.6183.77\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Dradozer\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\191.6183.77\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Desktop\Projekte\BA\ba_gaida\src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug

# Include any dependencies generated for this target.
include executables/ba_gaida/CMakeFiles/ba_gaida.dir/depend.make

# Include the progress variables for this target.
include executables/ba_gaida/CMakeFiles/ba_gaida.dir/progress.make

# Include the compile flags for this target's objects.
include executables/ba_gaida/CMakeFiles/ba_gaida.dir/flags.make

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/flags.make
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/includes_CXX.rsp
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj: ../executables/ba_gaida/include/Camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ba_gaida.dir\include\Camera.cpp.obj -c E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\Camera.cpp

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ba_gaida.dir/include/Camera.cpp.i"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\Camera.cpp > CMakeFiles\ba_gaida.dir\include\Camera.cpp.i

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ba_gaida.dir/include/Camera.cpp.s"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\Camera.cpp -o CMakeFiles\ba_gaida.dir\include\Camera.cpp.s

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/flags.make
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/includes_CXX.rsp
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj: ../executables/ba_gaida/include/FpsCounter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ba_gaida.dir\include\FpsCounter.cpp.obj -c E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\FpsCounter.cpp

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.i"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\FpsCounter.cpp > CMakeFiles\ba_gaida.dir\include\FpsCounter.cpp.i

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.s"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\FpsCounter.cpp -o CMakeFiles\ba_gaida.dir\include\FpsCounter.cpp.s

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/flags.make
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/includes_CXX.rsp
executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj: ../executables/ba_gaida/include/ParticleSystem.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ba_gaida.dir\include\ParticleSystem.cpp.obj -c E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\ParticleSystem.cpp

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.i"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\ParticleSystem.cpp > CMakeFiles\ba_gaida.dir\include\ParticleSystem.cpp.i

executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.s"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\include\ParticleSystem.cpp -o CMakeFiles\ba_gaida.dir\include\ParticleSystem.cpp.s

executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/flags.make
executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.obj: executables/ba_gaida/CMakeFiles/ba_gaida.dir/includes_CXX.rsp
executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.obj: ../executables/ba_gaida/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.obj"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ba_gaida.dir\main.cpp.obj -c E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\main.cpp

executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ba_gaida.dir/main.cpp.i"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\main.cpp > CMakeFiles\ba_gaida.dir\main.cpp.i

executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ba_gaida.dir/main.cpp.s"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida\main.cpp -o CMakeFiles\ba_gaida.dir\main.cpp.s

# Object files for target ba_gaida
ba_gaida_OBJECTS = \
"CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj" \
"CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj" \
"CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj" \
"CMakeFiles/ba_gaida.dir/main.cpp.obj"

# External object files for target ba_gaida
ba_gaida_EXTERNAL_OBJECTS =

bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/Camera.cpp.obj
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/FpsCounter.cpp.obj
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/include/ParticleSystem.cpp.obj
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/main.cpp.obj
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/build.make
bin/ba_gaida.exe: C:/dependencies_win32/glfw-3.0.4.bin.WIN32/lib-mingw/libglfw3.a
bin/ba_gaida.exe: C:/dependencies_win32/assimp/libMinGW/libassimp.dll
bin/ba_gaida.exe: C:/dependencies_win32/OpenGL/lib/glew32s.lib
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/linklibs.rsp
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/objects1.rsp
bin/ba_gaida.exe: executables/ba_gaida/CMakeFiles/ba_gaida.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ..\..\bin\ba_gaida.exe"
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ba_gaida.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
executables/ba_gaida/CMakeFiles/ba_gaida.dir/build: bin/ba_gaida.exe

.PHONY : executables/ba_gaida/CMakeFiles/ba_gaida.dir/build

executables/ba_gaida/CMakeFiles/ba_gaida.dir/clean:
	cd /d E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida && $(CMAKE_COMMAND) -P CMakeFiles\ba_gaida.dir\cmake_clean.cmake
.PHONY : executables/ba_gaida/CMakeFiles/ba_gaida.dir/clean

executables/ba_gaida/CMakeFiles/ba_gaida.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Desktop\Projekte\BA\ba_gaida\src E:\Desktop\Projekte\BA\ba_gaida\src\executables\ba_gaida E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida E:\Desktop\Projekte\BA\ba_gaida\src\cmake-build-debug\executables\ba_gaida\CMakeFiles\ba_gaida.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : executables/ba_gaida/CMakeFiles/ba_gaida.dir/depend

