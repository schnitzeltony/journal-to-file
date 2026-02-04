cmake_minimum_required(VERSION 3.10)

project(journal-to-file LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "1")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(QT_MAJOR_VER "5" CACHE STRING "Qt major version")
set(QTX "Qt${QT_MAJOR_VER}")

include(FeatureSummary)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

find_package(${QTX} COMPONENTS Core Test REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)

enable_testing()

write_basic_package_version_file(
    JournalToFileConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(JournalToFileConfig.cmake.in JournalToFileConfig.cmake @ONLY)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/JournalToFileConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/JournalToFileConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JournalToFile
    )

install(EXPORT JournalToFileExport
    FILE JournalToFileTargets.cmake
    NAMESPACE Zera::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JournalToFile
    )

add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
