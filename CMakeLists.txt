cmake_minimum_required(VERSION 3.6)
project(kinect-depth-png)

add_executable(${PROJECT_NAME} kinect.cpp buffer.cpp image.cpp kinect-depth-png.cpp)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(libfreenect REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/libfreenect)
target_link_libraries(${PROJECT_NAME} PRIVATE hdf5 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${FREENECT_LIBRARIES} png)