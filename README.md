sudo dnf install ninja cmake mesa-libGL-devel mesa-libGLU-devel glfw-devel

cmake -B build -G Ninja
cmake --build build
