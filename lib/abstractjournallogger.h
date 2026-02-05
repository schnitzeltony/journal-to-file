#ifndef ABSTRACTJOURNALLOGGER_H
#define ABSTRACTJOURNALLOGGER_H

#include <QObject>

class AbstractJournalLogger : public QObject
{
    Q_OBJECT
public:
    enum LinePriorities {
        EMERGENCY,
        ERROR,
        WARNING,
        NOTICE,
        INFO,
        DEBUG,
        AUDIT,
        UNKNOWN
    };
    AbstractJournalLogger();
    static QString getPrioText(LinePriorities prio);
    static void registerMeta();

signals:
    void sigLineAdd(const double &timeStamp,
                    AbstractJournalLogger::LinePriorities prio,
                    const QString &pid,
                    const QString &processName,
                    const QString &message);
};

#endif // ABSTRACTJOURNALLOGGER_H
