enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    add_definitions("-DBOOST_LOG_DYN_LINK")
    add_executable(picture_test picture_test.cpp)
    add_executable(csv_test csv_test.cpp)
    target_link_libraries(picture_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)
    target_link_libraries(csv_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    # Tests
    add_test("picture_test" picture_test ${TESTDATA_DIR} ${TESTDATA_DIR}/pictureimport_test_csv.csv --log_format=XML --log_sink=TEST_PICTURE.xml --log_level=all --report_level=no)
    add_test("csv_test" csv_test ${TESTDATA_DIR} ${TESTDATA_DIR}/pictureimport_test_csv.csv --log_format=XML --log_sink=TEST_CSV.xml --log_level=all --report_level=no)
endif()
