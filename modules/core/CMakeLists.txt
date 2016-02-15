include_directories(include ${EXIV2_INCLUDE_DIR})
add_library(Core ${EXIV2_LIBRARIES} src/target.cpp src/object.cpp src/pixel_object.cpp src/frame.cpp src/benchmark.cpp)
