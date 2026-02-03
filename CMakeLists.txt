cmake_minimum_required(VERSION 3.16)

project(journal-to-file LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)

add_executable(journal-to-file
  main.cpp
  journallogger.h journallogger.cpp
  journalwriter.h journalwriter.cpp
  abstractjournallogger.h
  abstractjournallogger.cpp
)
target_link_libraries(journal-to-file
    Qt${QT_VERSION_MAJOR}::Core
    systemd
)

include(GNUInstallDirs)
install(TARGETS journal-to-file
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
