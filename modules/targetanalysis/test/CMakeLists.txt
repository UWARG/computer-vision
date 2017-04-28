enable_testing()
include_directories(../include ../../core/include ../../targetid/include
../../imgimport/include)

if(Boost_FOUND AND OpenCV_FOUND)
    ADD_DEFINITIONS("-DBOOST_LOG_DYN_LINK")
    add_executable(target_loader_test test.cpp)
    add_executable(unique_po_test unique_po_test.cpp)
    add_executable(gps_point_test gps_point_test.cpp)
    target_link_libraries(target_loader_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core TargetAnalysis)
    target_link_libraries(unique_po_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core TargetAnalysis TargetIdentification)
    target_link_libraries(gps_point_test ${OpenCV_LIBS} ${Boost_LIBRARIES} Core
    TargetAnalysis ImageImport)

    # Tests
    add_test("SimpleLoad" target_loader_test "${TESTDATA_DIR}/sample.json" "Simple Load" --log_format=XML --log_sink=TEST_LOADER.xml --log_level=all --report_level=no)
    add_test("UniquePOTest" unique_po_test "${TESTDATA_DIR}/photos" "Pixel Object Uniqueness" --log_format=XML --log_sink=TEST_LOADER.xml --log_level=all --report_level=no)
    add_test("GPSPointTest" gps_point_test "${TESTDATA_DIR}/photos" "GPS Point Test" --log_format=XML --log_sink=TEST_LOADER.xml --log_level=all --report_level=no)

endif()
