cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)
project(mvBlueFoxWebcam VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

# Policies
cmake_policy(SET CMP0028 NEW) # Double colon in target name means ALIAS or IMPORTED target

set(MVIMPACT_DIR "/opt/mvIMPACT_Acquire")

include(${MVIMPACT_DIR}/mvIMPACT_AcquireConfig.cmake)
include(${MVIMPACT_DIR}/CMake/cpp.standard.detection.cmake)

# app
add_executable(webcam
        src/main.cpp
        src/AquireHelper.hpp
        src/CameraManager.cpp
        src/CameraManager.hpp
        src/Camera.cpp
        src/Camera.hpp
        src/CameraHelper.hpp
        src/CameraHelper.cpp src/Webcam.cpp src/Webcam.hpp src/FileHandleWrapper.cpp src/FileHandleWrapper.hpp)


target_include_directories(webcam PUBLIC
        ${mvIMPACT_Acquire_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(webcam
        pthread
        rt
        yuv
        ${mvIMPACT_Acquire_LIBRARIES}
        ${OpenCV_LIBS}
        )

