enable_testing()

if(Boost_FOUND AND OpenCV_FOUND)
    ADD_DEFINITIONS("-DBOOST_LOG_DYN_LINK")
    add_executable(targetid_test_contour test.cpp)
    target_link_libraries(targetid_test_contour ${OpenCV_LIBS} ${Boost_LIBRARIES} Core TargetIdentification)

    install(TARGETS targetid_test_contour DESTINATION bin)

    # Tests
    add_test("simple_target" targetid_test_contour "imgs/IMG_1644.jpg" "Simple target" 798 264 806 225 844 235 837 273)
endif()
