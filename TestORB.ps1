cmake -B build\TestORB -S TestORB --toolchain "$ENV:NIRVANA_SDK\cmake\nirvana.cmake" -G Ninja -DNIRVANA_TARGET_PLATFORM=x64
cmake --build build\TestORB
