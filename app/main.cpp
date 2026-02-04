#include "journallogger.h"
#include "journalwriter.h"
#include <QCoreApplication>
#include <iostream>
#include <systemd/sd-journal.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    sd_journal *journalHandle;
    if (sd_journal_open(&journalHandle, SD_JOURNAL_LOCAL_ONLY) < 0) {
        std::cerr << "Could not open journal!\n";
        return - 1;
    }

    std::shared_ptr<AbstractJournalLogger> logger = std::make_shared<JournalLogger>(journalHandle);
    JournalWriter writer(logger);

    int ret = app.exec();
    sd_journal_close(journalHandle);
    return ret;
}
