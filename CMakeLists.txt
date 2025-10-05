cmake_minimum_required(VERSION 3.16)
project(AcademicLedger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Попробуем найти Qt6, если не найдется - попробуем Qt5
find_package(Qt6 COMPONENTS Core Widgets QUIET)

# Если Qt6 не найден, попробуем Qt5
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets QUIET)
    if(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
        message(STATUS "Найден Qt5")
    else()
        message(FATAL_ERROR 
            "Qt не найден!\n\n"
            "Для сборки проекта необходимо установить Qt5 или Qt6.\n"
            "Инструкции по установке см. в файле INSTALL_QT.md\n\n"
            "Возможные решения:\n"
            "1. Установите Qt через официальный установщик\n"
            "2. Установите Qt через vcpkg\n"
            "3. Установите переменную CMAKE_PREFIX_PATH в CLion\n"
            "4. Перезапустите CLion после установки Qt"
        )
    endif()
else()
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Найден Qt6")
endif()

add_executable(AcademicLedger 
    main.cpp
    user.cpp
    datastore.cpp
    mainwindow.cpp
    logindialog.cpp
    registrationdialog.cpp
    studentview.cpp
    teacherview.cpp
    adminview.cpp
    init_demo_data.cpp
        mainwindow.cpp
)

# Copy stylesheet to build directory
configure_file(brutal_style.qss ${CMAKE_BINARY_DIR}/brutal_style.qss COPYONLY)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(AcademicLedger Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(AcademicLedger Qt5::Core Qt5::Widgets)
endif()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Find windeployqt reliably (works in CLion if CMAKE_PREFIX_PATH is set in Profile)
    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt windeployqt.exe
        HINTS
            "${CMAKE_PREFIX_PATH}/bin"
            "$ENV{CMAKE_PREFIX_PATH}/bin"
        PATHS
            "${CMAKE_PREFIX_PATH}/bin"
            "$ENV{CMAKE_PREFIX_PATH}/bin"
        PATH_SUFFIXES bin
    )

    if (WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-compiler-runtime --release "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt to deploy Qt runtime dependencies")
    else()
        message(STATUS "windeployqt not found. To run from CLion, add Qt bin to PATH or set CMAKE_PREFIX_PATH.")
    endif()
endif ()
