file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(journal-to-file-lib SHARED
    ${HEADER}
    ${SOURCES}
    )

target_include_directories(journal-to-file-lib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/journal-to-file-lib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(journal-to-file-lib
    PUBLIC
    ${QTX}::Core
    systemd
    )

set_target_properties(journal-to-file-lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(journal-to-file-lib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS journal-to-file-lib NAMESPACE Zera:: FILE JournalToFileTargets.cmake)
install(TARGETS journal-to-file-lib
    EXPORT JournalToFileExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/journal-to-file-lib
    )
