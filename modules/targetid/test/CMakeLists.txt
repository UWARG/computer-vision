enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    ADD_DEFINITIONS("-DBOOST_LOG_DYN_LINK")
    add_executable(targetid_test_contour test.cpp)
    target_link_libraries(targetid_test_contour ${OpenCV_LIBS} ${Boost_LIBRARIES} Core TargetIdentification)

    # Tests
    #    add_test(
    #   NAME "simple_target"
    #    COMMAND targetid_test_contour "Simple target" "${CMAKE_CURRENT_LIST_DIR}/imgs/IMG_1644.JPG" 798 264 806 225 844 235 837 273
    #)
    ADD_EXECUTABLE(UnitTester boosttest.cpp)
    TARGET_LINK_LIBRARIES(UnitTester ${Boost_LIBRARIES})
    ADD_TEST("Test1" UnitTester --log_format=XML --log_sink=TEST-boost.xml --log_level=all --report_level=no)
endif()
