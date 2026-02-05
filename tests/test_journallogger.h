#ifndef TEST_JOURNALLOGGER_H
#define TEST_JOURNALLOGGER_H

#include <QObject>

class test_journallogger : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void analyzeTestJournal();

private:
    void createTestJournal(); // to run make it a test (=move up)
    void feedEventLoop();
};

#endif // TEST_JOURNALLOGGER_H
