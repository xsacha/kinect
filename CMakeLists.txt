cmake_minimum_required(VERSION 3.0.2)

# hunter default config file in cmake/Hunter/config.cmake
#include(cmake/Hunter/HunterGate.cmake)
#HunterGate(
#    URL "https://github.com/ruslo/hunter/archive/v0.19.148.tar.gz"
#    SHA1 "63631441d472a0f8d80f8efefa02b544193aafea"
#    LOCAL
#)
project(kinect-depth-png)

#hunter_add_package(OpenCV)
#find_package(OpenCV REQUIRED)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui png)
set(FREENECT_FOUND TRUE)
set(FREENECT_LIBRARIES freenect)


find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} kinect.cpp buffer.cpp image.cpp kinect-depth-png.cpp)
find_package(libfreenect QUIET)
if(FREENECT_FOUND)
  # Linux
  target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/libfreenect)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${FREENECT_LIBRARIES})
else()
  # Windows
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libfreenect/include)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libfreenect/lib/freenect.lib)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
