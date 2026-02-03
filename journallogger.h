#ifndef JOURNALLOGGER_H
#define JOURNALLOGGER_H

#include "abstractjournallogger.h"
#include <QSocketNotifier>
#include <systemd/sd-journal.h>

class JournalLogger : public AbstractJournalLogger
{
    Q_OBJECT
public:
    JournalLogger(sd_journal *journalHandle);

private slots:
    void readEntries();
private:
    bool selectCurrentBoot();
    void seekNextHead();
    QString getFieldFromJournal(const QString &fieldName);
    QString getProcessDisplayedFromJournal();
    LinePriorities getPriorityFromJournal();

    static LinePriorities translatePriority(int systemPriority);
    static void removeTrailingControlChars(QString &line);

    sd_journal *m_journalHandle = nullptr;
    QString m_lastCursor;
    sd_id128_t m_bootId;
    std::unique_ptr<QSocketNotifier> m_notifier;
};

#endif
