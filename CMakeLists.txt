cmake_minimum_required(VERSION 3.10)
project(slib)
set(SLIB_DESC "simple lib for c.")

# include custom modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# set include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/inc)

# Version of the project
set(SLIB_MAJOR_VERSION 0)
set(SLIB_MINOR_VERSION 0)
set(SLIB_MICRO_VERSION 1)
set(SLIB_VERSION ${SLIB_MAJOR_VERSION}.${SLIB_MINOR_VERSION}.${SLIB_MICRO_VERSION})

# Version of the library
set(SLIB_MAJOR_SOVERSION 0)
set(SLIB_MINOR_SOVERSION 0)
set(SLIB_MICRO_SOVERSION 1)
set(SLIB_SOVERSION_FULL ${SLIB_MAJOR_SOVERSION}.${SLIB_MINOR_SOVERSION}.${SLIB_MICRO_SOVERSION})
set(SLIB_SOVERSION ${SLIB_MAJOR_SOVERSION})

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

# compilation flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O2")
set(CMAKE_C_FLAGS_PACKAGE "-g -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
message(STATUS "test${SRC_LIST}")

# add shared library
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
# set shared library's version
# VERSION: shared library's version, SOVERSION:API's version
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${SLIB_VERSION} SOVERSION 0)

add_library(${PROJECT_NAME}_static STATIC ${SRC_LIST})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

include(GNUInstallDirs)

# generate files
configure_file("${PROJECT_SOURCE_DIR}/slib.pc.in" "${PROJECT_BINARY_DIR}/slib.pc" @ONLY)

file(GLOB HD_LIST ${PROJECT_SOURCE_DIR}/include/slib/*.h)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HD_LIST} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/slib)
install(FILES "${PROJECT_BINARY_DIR}/slib.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")