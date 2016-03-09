enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    add_definitions("-DBOOST_LOG_DYN_LINK")
    add_executable(picture_import_test test.cpp)
    target_link_libraries(picture_import_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core ImageImport)

    # Tests
    add_test("picture_test" picture_import_test ${TESTDATA_DIR}/IMG_1644.JPG ${TESTDATA_DIR}/pictureimport_test_csv.csv --log_format=XML --log_sink=TEST-boost.xml --log_level=all --report_level=no)
    add_test("csv_test" picture_import_test ${TESTDATA_DIR}/IMG_1644.JPG ${TESTDATA_DIR}/pictureimport_test_csv.csv --log_format=XML --log_sink=TEST-boost.xml --log_level=all --report_level=no)
endif()
