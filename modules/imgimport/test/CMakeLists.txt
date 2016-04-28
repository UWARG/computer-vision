enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    add_executable(picture_test picture_test.cpp)
    target_link_libraries(picture_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    add_executable(video_test video_test.cpp)
    target_link_libraries(video_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    add_executable(manual_calibration manual_calibration.cpp)
    target_link_libraries(manual_calibration ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    install(TARGETS manual_calibration DESTINATION bin)

    # Tests
    add_test("picture_test" picture_test ${TESTDATA_DIR}/photos ${TESTDATA_DIR} --log_format=XML --log_sink=TEST_PICTURE.xml --log_level=all --report_level=no)
    add_test("video_test" video_test ${TESTDATA_DIR}/vid.mp4 --log_format=XML --log_sink=TEST_VIDEO.xml --log_level=all --report_level=no)

    if (DeckLinkSDK_FOUND)
        ADD_DEFINITIONS("-DHAS_DECKLINK")
        add_executable(target_video_test test.cpp ${DeckLinkSDK_INCLUDE_PATH}/DeckLinkAPIDispatch.cpp)
        target_link_libraries(target_video_test ${OpenCV_LIBS} ${Boost_LIBRARIES} ${DeckLinkSDK_LIBS} ${Boost_FILESYSTEM_LIBRARY} Core ImageImport)
        add_test("video_load" target_video_test "Simple Video Load" --log_format=XML --log_sink=TEST_VIDEO.xml --log_level=all --report_level=no)
    endif()
endif()
