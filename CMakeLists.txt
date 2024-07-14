cmake_minimum_required(VERSION 3.5)

set(UNIT_NAME PersonalFinanceManager)
project(${UNIT_NAME} VERSION 0.1 LANGUAGES CXX)

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



        AuthenticationModule/AuthenticationModule.h
        AuthenticationModule/AuthenticationModule.cpp
        DataManagementModule/DataManagementModule.h
        DataManagementModule/DataManagementModule.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message(STATUS "Used QT version is greater than 6, other parts of cmake is deleted!")
    qt_add_executable(${UNIT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


        UIModule/stackedwindows.h UIModule/stackedwindows.cpp UIModule/stackedwindows.ui



    )
endif()

target_link_libraries(${UNIT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${UNIT_NAME} PUBLIC sqlite3)
target_include_directories(${UNIT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

add_subdirectory(test)


include(GNUInstallDirs)
install(TARGETS ${UNIT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${UNIT_NAME})
endif()
