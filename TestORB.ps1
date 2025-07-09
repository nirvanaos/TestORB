cmake -B build/TestORB -S TestORB --toolchain "$PWD\nirvana.cmake" -G Ninja
cmake --build build/TestORB
