find_path(Jsoncpp_INCLUDE_DIR
  NAMES json/features.h
  PATH_SUFFIXES jsoncpp
  PATHS ${Jsoncpp_PKGCONF_INCLUDE_DIRS}
)

include_directories(include ../core/include ${Jsoncpp_INCLUDE_DIR})
add_library(TargetAnalysis ${Jsoncpp_LIBRARY} src/area_analyzer.cpp src/target_analyzer.cpp src/target_loader.cpp)
