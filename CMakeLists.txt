cmake_minimum_required(VERSION 3.6)
project(cpa-sim)


find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/scamp.h src/scamp.cpp src/scamp_def.h src/main.cpp src/sim.cpp src/sim.h)
add_executable(cpa-sim ${SOURCE_FILES})
target_link_libraries( cpa-sim ${OpenCV_LIBS} )