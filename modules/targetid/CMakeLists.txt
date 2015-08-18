include_directories(include ../core/include)
add_library(TargetIdentification src/target_identifier.cpp src/object_detector.cpp )
target_link_libraries(TargetIdentification Core)
target_compile_features(TargetIdentification PRIVATE cxx_range_for)
