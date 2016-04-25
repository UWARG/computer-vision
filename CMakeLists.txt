cmake_minimum_required(VERSION 3.0)

project(warg-cv CXX C)

set(MODULES core csvcreator targetanalysis targetid volumecalc reportgen imgimport gmapsmatcher)

#Adds a custom FIND<MODULE>.CMAKE folder to the project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Dependencies
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED filesystem log thread system program_options unit_test_framework)
find_package(Threads)
find_package(DeckLinkSDK)
find_package(Exiv2 REQUIRED)

message(status " Boost Include: ${Boost_INCLUDE_DIR}")
message(status " Boost Libraries: ${Boost_LIBRARIES}")
message(status " OpenCV Include: ${OpenCV_INCLUDE_DIRS}")
message(status " OpenCV Libraries: ${OpenCV_LIBS}")
message(status " DeckLink Include: ${DeckLinkSDK_INCLUDE_PATH}")
message(status " DeckLink Libraries: ${DeckLinkSDK_LIBS}")
message(status " Exiv2 Include: ${EXIV2_INCLUDE_DIR}")
message(status " Exiv2 Libraries: ${EXIV2_LIBRARIES}")

if(NOT Threads_FOUND)
    message(warning " Threads library not found. Proceed with caution")
endif()

set(CMAKE_CXX_STANDARD 11)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

# Boost seems to require pthread on some systems
# TODO: Find out what is causing this requirement
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set(TESTDATA_DIR ${CMAKE_CURRENT_LIST_DIR}/testdata)

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
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native")
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

    if(DeckLinkSDK_FOUND)
        include_directories(${DeckLinkSDK_INCLUDE_PATH})    	
        ADD_DEFINITIONS("-DHAS_DECKLINK")
	SET(COMPILE_DEFINITIONS "${DeckLinkSDK_DEF}")
    else(DeckLinkSDK_FOUND)
	message(status " DeckLinkSDK not found. Omitting DeckLink features.")
    endif(DeckLinkSDK_FOUND)

    if(CMAKE_BUILD_TYPE STREQUAL Release)
        add_definitions("-DRELEASE")
    else()
        add_definitions("-DDEBUG")
    endif()
    add_executable(warg-cv main.cpp)
    target_link_libraries(warg-cv ${Boost_LIBRARIES} Core ReportGenerator CSVCreator TargetAnalysis TargetIdentification VolumeCalculation ImageImport GoogleMapsMatcher)
    install(TARGETS warg-cv DESTINATION bin)

endif()

