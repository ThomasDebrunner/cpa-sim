cmake_minimum_required(VERSION 3.6)
project(cpa-sim)

OPTION(SUPER_DEBUG "Super Debug" OFF)
IF(SUPER_DEBUG)
    ADD_DEFINITIONS(-DSUPER_DEBUG)
ENDIF(SUPER_DEBUG)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)

AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)

set(SOURCE_FILES ${SOURCE_FILES} src/scamp.h src/scamp.cpp
        src/scamp_def.h
        src/main.cpp src/sim.cpp
        src/sim.h
        src/demos/rot_scale.cpp
        src/demos/rot_scale.h
        src/scamp_access.cpp
        src/scamp_access.h)
add_executable(cpa-sim ${SOURCE_FILES})
target_link_libraries( cpa-sim ${OpenCV_LIBS} )