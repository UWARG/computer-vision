include_directories(include ../core/include)
add_library(ImageImport src/imgimport.cpp)
target_link_libraries(ImageImport ${OpenCV_LIBS} ${Boost_LIBRARIES} Core)
