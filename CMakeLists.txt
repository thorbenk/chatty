project(chatty)
cmake_minimum_required(VERSION 2.8)
enable_testing()

add_definitions(-std=c++0x)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_MODULE_PATH "${chatty_SOURCE_DIR}/config")

find_package(Boost
    COMPONENTS python system filesystem date_time serialization unit_test_framework regex thread
    REQUIRED
)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
#try to guess the site-packages path
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" 
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_CMD OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT PYTHON_SITE_PACKAGES)
    #if site-packages location was specified on the command line, use that
    #instead
    SET(PYTHON_SITE_PACKAGES ${PYTHON_SITE_PACKAGES_CMD} CACHE TYPE "Where to install python modules")
endif()

include_directories(
    ${chatty_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
)

add_definitions(-std=c++0x)

add_subdirectory(src)
add_subdirectory(python)
add_subdirectory(test)
add_subdirectory(examples)

