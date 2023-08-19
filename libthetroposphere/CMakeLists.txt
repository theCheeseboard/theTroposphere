cmake_minimum_required(VERSION 3.4.0)

project(lib VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
find_package(libcontemporary REQUIRED)

set(SOURCES
    tropospherelocation.cpp
    weatherdata.cpp
    weathertimeseries.cpp
)

set(HEADERS
    tropospherelocation.h
    weatherdata.h
    weathertimeseries.h
    libthetroposphere_global.h
)

set(PRIVATE_HEADERS

)

add_library(libthetroposphere SHARED ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS})
cntp_init(libthetroposphere 20)
set_target_properties(libthetroposphere PROPERTIES
        OUTPUT_NAME thetroposphere
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.thetroposphere
        VERSION 1.0.0
        PUBLIC_HEADER "${HEADERS}")

target_link_libraries(libthetroposphere Qt::Widgets Qt::Network libcontemporary)
target_compile_definitions(libthetroposphere PRIVATE LIBTHETROPOSPHERE_LIBRARY)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/libthetroposphere.framework/Resources/CMake)
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/libthetroposphere.framework/Headers)
    set(LIBRARY_INSTALL_DIR ../)
ELSE()
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/libthetroposphere)
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/libthetroposphere)
    set(INCLUDE_INSTALL_DIR ${HEADER_INSTALL_DIR})
    set(LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
ENDIF()

configure_package_config_file(libthetroposphereConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/libthetroposphereConfig.cmake
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        PATH_VARS HEADER_INSTALL_DIR LIBRARY_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libthetroposphereConfig.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(EXPORT libthetroposphereTargets
        FILE libthetroposphereTargets.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS libthetroposphere
        EXPORT libthetroposphereTargets
        PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_DIR}
        INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})

cntp_mark_support_library_directory()
