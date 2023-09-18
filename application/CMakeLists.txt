cmake_minimum_required(VERSION 3.21.0)

project(thetroposphere VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Positioning Network)
find_package(libcontemporary REQUIRED)

set(SOURCES
        main.cpp
        mainwindow.cpp mainwindow.ui
        weatherpane.cpp weatherpane.ui
        addlocationpopover.cpp addlocationpopover.ui
        weather/weatherwidget.cpp weather/weatherwidget.ui
        weather/temperaturetimewidget.cpp weather/temperaturetimewidget.ui
        tropospherehelper.cpp

        settings/temperatureunitsettingspane.cpp settings/temperatureunitsettingspane.ui
)

set(HEADERS
        mainwindow.h
        weatherpane.h
        addlocationpopover.h
        tropospherehelper.h
        weather/weatherwidget.h
        weather/temperaturetimewidget.h

        settings/temperatureunitsettingspane.h
)

# Find Apple lproj files
set(LPROJ_SOURCES)
file(GLOB LPROJ_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/translations/apple-lproj/*.lproj)
FOREACH(DIR ${LPROJ_DIRS})
    cmake_path(GET DIR FILENAME DIR_NAME)
    file(GLOB LPROJ_FILES ${DIR}/*.strings)
    FOREACH(FILE ${LPROJ_FILES})
        list(APPEND LPROJ_SOURCES ${FILE})
        set_source_files_properties(${FILE} PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources/${DIR_NAME})
    ENDFOREACH()
ENDFOREACH()
set(SOURCES ${SOURCES} ${LPROJ_SOURCES})

add_executable(thetroposphere ${SOURCES} ${HEADERS})

set_target_properties(thetroposphere PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        MACOSX_BUNDLE_GUI_IDENTIFIER com.vicr123.thetroposphere
        WIN32_EXECUTABLE TRUE
        CNTP_DATA_SUBDIR thetroposphere)

cntp_init(thetroposphere 20)
cntp_translate(thetroposphere)
cntp_target_name(thetroposphere "theTroposphere")
cntp_app_icon(thetroposphere
    BASEICON icons/baseicon.svg
    BASEICON_MAC icons/baseicon-mac.svg)
cntp_target_desktop_file(thetroposphere
    DESKTOP_FILE com.vicr123.thetroposphere.desktop
    BLUEPRINT_DESKTOP_FILE com.vicr123.thetroposphere_blueprint.desktop)
cntp_defaults_file(thetroposphere
    DEFAULTS_FILE thetroposphere.conf)

target_link_libraries(thetroposphere Qt::Widgets Qt::Positioning Qt::Network libcontemporary libthetroposphere)
target_include_directories(thetroposphere PUBLIC ../libthetroposphere/)

install(TARGETS thetroposphere
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION /Applications)
