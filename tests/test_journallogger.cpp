#include "test_journallogger.h"
#include "journallogger.h"
#include "journalwriter.h"
#include <systemd/sd-journal.h>
#include <QDir>
#include <QThread>
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_journallogger)

static const char* journalpath = "/tmp/test_journallogger";
static const char* journal = "/tmp/test_journallogger/test_logs.journal";

void test_journallogger::init()
{
    QDir dir;
    QVERIFY(dir.mkpath(journalpath));
    QFile::copy(":/testjournal/test_logs.journal", journal);
}

void test_journallogger::cleanup()
{
    QDir dir(journalpath);
    dir.removeRecursively();
}

void test_journallogger::analyzeTestJournal()
{
    sd_journal *journalHandle;
    int ret = sd_journal_open_directory(&journalHandle, journalpath, 0);
    QCOMPARE(ret, 0);

    JournalLogger logger(journalHandle);
    QSignalSpy spy(&logger, &JournalLogger::sigLineAdd);
    feedEventLoop();
    QCOMPARE(spy.size(), 101);
    QCOMPARE(spy[0][3], "sudo");
    QCOMPARE(spy[1][1], AbstractJournalLogger::WARNING);
    QCOMPARE(spy[1][3], "test_journallogger");
    QCOMPARE(spy[1][4], "0");
    QCOMPARE(spy[2][4], "1");
}


void test_journallogger::createTestJournal()
{
    // run sudo journalctl --rotate
    // copy /var/log/journal/<user-id>/user-1000.journal -> :/testjournal/test_logs.journal
    for (int i=0; i<100; i++) {
        QString txt = QString("%1").arg(i);
        sd_journal_print(LOG_WARNING, "%s", qPrintable(txt.toLocal8Bit()));
        QTest::qWait(1);
    }
}

void test_journallogger::feedEventLoop()
{
    while( QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents) );
}
