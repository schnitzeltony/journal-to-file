add_executable(journal-to-file
    main.cpp
    )

target_link_libraries(journal-to-file
    PUBLIC
    journal-to-file-lib
    )

install(TARGETS journal-to-file
    EXPORT JournalToFileExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

