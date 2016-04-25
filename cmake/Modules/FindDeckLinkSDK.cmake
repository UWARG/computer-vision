# This module finds where the DeckLink SDK include file is installed.
# DeckLink SDK is the toolkit for BlackMagickDesign Capture hardware.
# Visit www.blackmagic-design.com/support
#
# This code sets the following variables:
#  DeckLinkSDK_FOUND    = DeckLinkSDK was found
#  DeckLinkSDK_PATH     = where DeckLinkSDK is. Cashed
#  DeckLinkSDK_INCLUDE_PATH = path to where ${DeckLinkSDK_INCLUDE_FILE} is found
#  DeckLinkSDK_INCLUDE_FILE = DeckLinkAPI.idl (Win) or .h (Mac/Linux)
#  DeckLinkSDK_SRC_FILE = full path to DeckLinkAPIDispatch.cpp (Mac/Linux) or empty (Win)
#  DeckLinkSDK_LIBS     = external libraries to link for target.
#
# Typical CmakeLists.txt snippet is;
#    IF(DeckLinkSDK_FOUND)
#      INCLUDE_DIRECTORIES("${DeckLinkSDK_INCLUDE_PATH}")
#      SET(COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS};${DeckLinkSDK_DEF}")
#      TARGET_LINK_LIBRARIES(${your_app} ${your_libs} "${DeckLinkSDK_LIBS}")
#      ADD_EXECUTABLE(${your_app} ${your_srcs} "${DeckLinkSDK_SRC_FILE}")
#    ENDIF(DeckLinkSDK_FOUND)
# You do the following in your DeckLink related sources;
#    #include "DeckLinkSDK_INCLUDE_FILE"

#=============================================================================
# (c) Kiyoyuki Chinzei, AIST japan.
# Intelligent Surgical Instrument Project / NEDO P100003
# Do not remove this copyright claim and the condition of the use.
# Use, modify and distribute it freely, but at your own risk.
#=============================================================================

SET(DeckLink_DEFAULT "/usr/local/include/DeckLinkSDK")

IF(WIN32)
    SET(DeckLinkSDK_SRC_FILE "")
    SET(DeckLinkSDK_LIBS "")
    SET(DeckLinkSDK_INCLUDE_FILE DeckLinkAPI.idl)
    SET(DeckLinkSDK_INCLUDE_PATH Win/include)
ELSE(WIN32)
    SET(DeckLinkSDK_SRC_FILE DeckLinkAPIDispatch.cpp)
    SET(DeckLinkSDK_INCLUDE_FILE DeckLinkAPI.h)
    IF(APPLE)
	SET(DeckLinkSDK_INCLUDE_PATH Mac/include)
	FIND_LIBRARY(DeckLinkSDK_LIBS CoreFoundation)
    ELSE(APPLE)
	SET(DeckLinkSDK_INCLUDE_PATH Linux/include)
	SET(DeckLinkSDK_LIBS "")
	FIND_LIBRARY(DeckLinkSDK_LIBS DeckLinkAPI
			PATHS /usr/lib /usr/local/lib $ENV{BLACKMAGIC_DIR}/lib/)
    ENDIF(APPLE)
ENDIF(WIN32)
MARK_AS_ADVANCED(FORCE DeckLinkSDK_LIBS)

# We attempt to examine with or without ${DeckLink_DEFAULT}
# We need to tempolariy unset ${DeckLinkSDK_PATH} cache.
if(DeckLinkSDK_PATH)
  SET(DeckLinkSDK_PATH_TMP "${DeckLinkSDK_PATH}")
else(DeckLinkSDK_PATH)
  SET(DeckLinkSDK_PATH_TMP "${DeckLink_DEFAULT}")
endif(DeckLinkSDK_PATH)

UNSET(DeckLinkSDK_PATH CACHE)
FIND_PATH(DeckLinkSDK_PATH
    ${DeckLinkSDK_INCLUDE_FILE}
    PATHS "${DeckLinkSDK_PATH_TMP}"
    PATH_SUFFIXES ${DeckLinkSDK_INCLUDE_PATH}
   )
UNSET(DeckLinkSDK_PATH_TMP)

IF(EXISTS "${DeckLinkSDK_PATH}/${DeckLinkSDK_INCLUDE_FILE}")
    SET(DeckLinkSDK_INCLUDE_PATH "${DeckLinkSDK_PATH}")
    IF(DeckLinkSDK_SRC_FILE)
	SET(DeckLinkSDK_SRC_FILE "${DeckLinkSDK_PATH}/${DeckLinkSDK_SRC_FILE}")
    ENDIF(DeckLinkSDK_SRC_FILE)
    SET(DeckLinkSDK_PATH
      "${DeckLinkSDK_PATH}"
      CACHE PATH "Where DeckLinkSDK locates.")
#    MARK_AS_ADVANCED(
#      DeckLinkSDK_INCLUDE_PATH
#      DeckLinkSDK_INCLUDE_FILE
#      DeckLinkSDK_SRC_FILE)
ELSE()    
    UNSET(DeckLinkSDK_INCLUDE_PATH)
    UNSET(DeckLinkSDK_SRC_FILE)
    UNSET(DeckLinkSDK_PATH CACHE)
    SET(DeckLinkSDK_PATH
      DeckLinkSDK-NOTFOUND
      CACHE PATH "Where DeckLinkSDK locates.")
ENDIF()

# Handle the QUIETLY and REQUIRED arguments and set DeckLinkSDK_FOUND to TRUE if 
# all listed variables are TRUE

INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  DeckLinkSDK 
  DEFAULT_MSG
  DeckLinkSDK_PATH)
SET(DeckLinkSDK_FOUND ${DECKLINKSDK_FOUND})
