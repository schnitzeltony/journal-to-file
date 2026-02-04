#include "journalwriter.h"

JournalWriter::JournalWriter(std::shared_ptr<AbstractJournalLogger> journalLogger) :
    m_journalLogger(journalLogger)
{
    connect(m_journalLogger.get(), &AbstractJournalLogger::sigLineAdd,
            this, &JournalWriter::onLineAdded);
}

void JournalWriter::onLineAdded(const double &timeStamp,
                                AbstractJournalLogger::LinePriorities prio,
                                const QString &pid,
                                const QString &processName,
                                const QString &message)
{
    // [timestamp] <prio> processName[pid]: message
    QString tsDisplay = QString("[%1]").arg(timeStamp, 12, 'f', 6, ' ');
    QString fullLine = tsDisplay + " " + AbstractJournalLogger::getPrioText(prio);
    fullLine += " " + processName;
    if (!pid.isEmpty())
        fullLine += "[" + pid + "]";
    fullLine += ":";
    fullLine += " " + message;

    if (!message.isEmpty()) {
        qWarning("%s", qPrintable(fullLine));
        /*QTextStream out(&m_outputFile);
            out << fullLine << "\n";
            out.flush();*/
    }
}
