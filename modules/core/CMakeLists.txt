include_directories(include ${EXIV2_INCLUDE_DIR})
add_library(Core  src/target.cpp src/object.cpp src/pixel_object.cpp src/frame.cpp src/benchmark.cpp src/camera.cpp)
target_link_libraries(Core ${EXIV2_LIBRARIES} ${OpenCV_LIBS})
