add_library(_chatty SHARED
    pyLogger.cxx
)
target_link_libraries(_chatty
    ${Boost_PYTHON_LIBRARIES}
    chatty
)
set_target_properties(_chatty PROPERTIES PREFIX "")

#add_test(NAME python_test
#    COMMAND ${PYTHON_EXECUTABLE} ${chatty_SOURCE_DIR}/test/yyy.py
#)
add_test(NAME python_test
    COMMAND ${PYTHON_EXECUTABLE} ${chatty_SOURCE_DIR}/test/test_logger.py
)
set_property(TEST python_test PROPERTY
    ENVIRONMENT LD_LIBRARY_PATH=${chatty_BINARY_DIR}/src:$ENV{LD_LIBRARY_PATH}
                PYTHONPATH=${chatty_BINARY_DIR}/python:$ENV{PYTHONPATH}
)
#ENVIRONMENT "LD_LIBRARY_PATH=${chatty_BUILD_DIR}/src:${chatty_BUILD_DIR}/python:$ENV{LD_LIBRARY_PATH}"

install(TARGETS _chatty LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}/chatty)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${PYTHON_SITE_PACKAGES}/chatty)

