cmake -B build\TestORB -S TestORB --toolchain "$ENV:NIRVANA_SDK\cmake\nirvana.cmake" -G Ninja `
 -DNIRVANA_TARGET_PLATFORM=x64 -DCMAKE_BUILD_TYPE=Debug

cmake --build build\TestORB
