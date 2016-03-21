enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    add_definitions("-DBOOST_LOG_DYN_LINK")
    add_executable(qr_test qr_test.cpp)
    add_executable(targetid_test_contour test.cpp)
    target_link_libraries(targetid_test_contour ${OpenCV_LIBS} ${Boost_LIBRARIES} Core TargetIdentification)
    target_link_libraries(qr_test ${OpenCV_LIBS} ${Boost_LIBRARIES} ${ZBAR_LIBRARIES} Core TargetIdentification)

    # Tests
    add_test("SimpleTarget" targetid_test_contour ${TESTDATA_DIR}/IMG_1644.JPG --log_format=XML --log_sink=TEST-boost.xml --log_level=all --report_level=no)
    add_test("qr_test" qr_test ${TESTDATA_DIR} --log_format=XML --log_sink=TEST-QR.xml --log_level=all --report_level=no)
endif()
