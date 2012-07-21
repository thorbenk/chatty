add_executable(test_logger test_logger.cxx)
target_link_libraries(test_logger
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    chatty
)
add_test(test_logger ${CMAKE_CURRENT_BINARY_DIR}/test_logger)

