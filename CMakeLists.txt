cmake_minimum_required(VERSION 3.5)

project(UIModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/halil/Qt/6.7.0/gcc_64/lib/cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#find_package(SQLite3 REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        UIModule/entrancewindow.cpp
        UIModule/entrancewindow.h
        UIModule/entrancewindow.ui
        AuthenticationModule/AuthenticationModule.h
        AuthenticationModule/AuthenticationModule.cpp
        DataManagementModule/DataManagementModule.h
        DataManagementModule/DataManagementModule.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message(STATUS "Used QT version is greater than 6, other parts of cmake is deleted!")
    qt_add_executable(UIModule
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(UIModule PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(UIModule PUBLIC sqlite3)
target_include_directories(UIModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.UIModule)
endif()
set_target_properties(UIModule PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS UIModule
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UIModule)
endif()
