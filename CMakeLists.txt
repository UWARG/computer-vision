cmake_minimum_required(VERSION 3.0)

project(warg-cv) 
# DEPENDENCIES
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

add_executable(warg-cv main.cpp)
set(MODULES CSV-creator target-analysis target-identification volume-calculation report-generator image-metadata-matcher google-maps-matcher)

foreach(p ${MODULES})
    add_subdirectory("src/${p}")
endforeach()

enable_testing()

foreach(p ${MODULES})
    include_directories("include/${p}")
endforeach()
target_link_libraries(warg-cv ${OpenCV_LIBS} ReportGenerator CSVCreator TargetAnalysis TargetIdentification VolumeCalculation MetadataMatcher GoogleMapsMatcher)
