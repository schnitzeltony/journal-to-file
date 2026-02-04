#ifndef JOURNALWRITER_H
#define JOURNALWRITER_H

#include "abstractjournallogger.h"
#include <memory>

class JournalWriter : public QObject
{
    Q_OBJECT
public:
    explicit JournalWriter(std::shared_ptr<AbstractJournalLogger> journalLogger);

private slots:
    void onLineAdded(const double &timeStamp,
                     AbstractJournalLogger::LinePriorities prio,
                     const QString &pid,
                     const QString &processName,
                     const QString &message);
private:
    std::shared_ptr<AbstractJournalLogger> m_journalLogger;
};

#endif // JOURNALWRITER_H
