cmake_minimum_required(VERSION 3.5)
set(CMAKE_GNUtoMS ON)

project(BUAASoftwareEngineeringPairProgrammingHWGui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets PrintSupport LinguistTools REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(TS_FILES BUAASoftwareEngineeringPairProgrammingHWGui_zh_CN.ts)

if(ANDROID)
  add_library(BUAASoftwareEngineeringPairProgrammingHWGui SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qcustomplot.cpp
    qcustomplot.h
    qcpitemhalfline.cpp
    qcpitemhalfline.h
    ${TS_FILES}
  )
else()
  add_executable(BUAASoftwareEngineeringPairProgrammingHWGui
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qcustomplot.cpp
    qcustomplot.h
    qcpitemhalfline.cpp
    qcpitemhalfline.h
    ${TS_FILES}
  )
endif()

find_library(LIBGEOMETRY_LIB gCore HINTS ${PROJECT_SOURCE_DIR}/lib)
if( ${LIBGEOMETRY_LIB} STREQUAL "LIBGEOMETRY_LIB-NOTFOUND")
    message("not found LIBGEOMETRY_LIB variable")
endif()
message("libgeometry dir: ${LIBGEOMETRY_LIB}")

include_directories("${PROJECT_SOURCE_DIR}/include")

target_link_libraries(BUAASoftwareEngineeringPairProgrammingHWGui
    Qt5::Widgets
    Qt5::PrintSupport
    ${LIBGEOMETRY_LIB}
    )

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
