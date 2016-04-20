enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    add_definitions("-DBOOST_LOG_DYN_LINK")
    add_executable(picture_test picture_test.cpp)
    target_link_libraries(picture_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    # Tests
    add_test("picture_test" picture_test ${TESTDATA_DIR}/photos ${TESTDATA_DIR} --log_format=XML --log_sink=TEST_PICTURE.xml --log_level=all --report_level=no)
endif()
