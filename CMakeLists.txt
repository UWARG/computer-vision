cmake_minimum_required(VERSION 3.0)

project(warg-cv CXX) 

set(MODULES core csvcreator targetanalysis targetid volumecalc reportgen imgimport gmapsmatcher)

# Dependencies
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED filesystem log thread system program_options)

message(status " Boost Include: ${Boost_INCLUDE_DIR}")
message(status " Boost Libraries: ${Boost_LIBRARIES}")
message(status " OpenCV Include: ${OpenCV_INCLUDE_DIRS}")
message(status " OpenCV Libraries: ${OpenCV_LIBS}")

set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

enable_testing()

# Include Modules

foreach(p ${MODULES})
    add_subdirectory("modules/${p}")
endforeach()

foreach(p ${MODULES})
    include_directories("modules/${p}/include")
endforeach()

if (UNIX)
    message(status "Setting GCC flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fexceptions -Wall -Wextra")
else()
    message(status "Setting MSVC flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHc-")
endif()
message(status " CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(status " CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

if(Boost_FOUND AND OpenCV_FOUND)
    ADD_DEFINITIONS("-DBOOST_LOG_DYN_LINK")
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${OpenCV_INCLUDE_DIRS})
    add_executable(warg-cv main.cpp)
    target_link_libraries(warg-cv ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ReportGenerator CSVCreator TargetAnalysis TargetIdentification VolumeCalculation ImageImport GoogleMapsMatcher)
endif()
