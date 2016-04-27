include_directories(include ../core/include)
ADD_DEFINITIONS("-DBOOST_LOG_DYN_LINK")

if (DeckLinkSDK_FOUND)
    ADD_DEFINITIONS("-DHAS_DECKLINK")
    include_directories(${DeckLinkSDK_INCLUDE_PATH})
    set(DeckLinkFiles ${DeckLinkSDK_LIBS} ${DeckLinkSDK_SRC_FILE} src/DeckLinkCapture.cpp src/DeckLinkOpenCv.cpp src/DeckLinkInputCallback.cpp)
endif (DeckLinkSDK_FOUND)

add_library(ImageImport src/imgimport.cpp src/decklink_import.cpp src/pictureimport.cpp src/metadata_input.cpp ${DeckLinkFiles})
target_link_libraries(ImageImport ${Boost_LIBRARIES} Core)

add_subdirectory("test")

