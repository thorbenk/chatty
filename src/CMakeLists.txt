add_library(chatty SHARED
    logentry.cxx
    logger.cxx
    logutils.cxx
    logline.cxx
    logtimedline.cxx
    logloop.cxx
    logtable.cxx
)
target_link_libraries(chatty
    rt
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_THREAD_LIBRARY}
)
install(TARGETS chatty
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
file(GLOB CHATTY_HEADERS ${chatty_SOURCE_DIR}/include/chatty/*.hxx)
install(FILES ${CHATTY_HEADERS} DESTINATION "include/chatty")
