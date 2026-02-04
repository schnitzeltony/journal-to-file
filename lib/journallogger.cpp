#include "journallogger.h"

JournalLogger::JournalLogger(sd_journal *journalHandle) :
    m_journalHandle(journalHandle)
{
    if(!selectCurrentBoot()) {
        qCritical("Could select current boot!");
        return;
    }

    sd_journal_seek_head(m_journalHandle);

    int fd = sd_journal_get_fd(m_journalHandle);
    m_notifier = std::make_unique<QSocketNotifier>(fd, QSocketNotifier::Read, this);
    connect(m_notifier.get(), &QSocketNotifier::activated,
            this, &JournalLogger::readEntries);

    QMetaObject::invokeMethod(this, "readEntries", Qt::QueuedConnection);
}

void JournalLogger::readEntries()
{
    bool handleMoved = false;
    while (sd_journal_next(m_journalHandle) > 0) {
        uint64_t usec;
        if (sd_journal_get_monotonic_usec(m_journalHandle, &usec, &m_bootId) < 0)
            continue;

        double seconds = usec / 1000000.0;
        QString message     = getFieldFromJournal("MESSAGE");
        QString processName = getProcessDisplayedFromJournal();
        QString pid         = getFieldFromJournal("_PID");
        LinePriorities prio = getPriorityFromJournal();

        handleMoved = true;
        emit sigLineAdd(seconds, prio, pid, processName, message);
    }
    if (handleMoved) {
        // more on cursors see [1]
        // https://www.freedesktop.org/software/systemd/man/latest/sd_journal_get_cursor.html
        char *cursorPtr = nullptr;
        if (sd_journal_get_cursor(m_journalHandle, &cursorPtr) >= 0) {
            m_lastCursor = QString::fromUtf8(cursorPtr);
            free(cursorPtr);
        }
    }
    if (sd_journal_process(m_journalHandle) == SD_JOURNAL_INVALIDATE)
        seekNextHead();
}

bool JournalLogger::selectCurrentBoot()
{
    if (sd_id128_get_boot(&m_bootId) < 0)
        return false;

    char boot_str[SD_ID128_STRING_MAX];
    sd_id128_to_string(m_bootId, boot_str);

    QString matchConstraint = QString("_BOOT_ID=%1").arg(boot_str);
    sd_journal_add_match(m_journalHandle, matchConstraint.toUtf8().constData(), 0);

    return true;
}

void JournalLogger::seekNextHead()
{
    if (!m_lastCursor.isEmpty()) {
        sd_journal_seek_cursor(m_journalHandle, m_lastCursor.toUtf8().constData());
        // 'Note that these calls do not actually make any entry the new current entry,
        // this needs to be done in a separate step with a subsequent sd_journal_next(3) invocation
        // (or a similar call).' [2]
        // [2] https://www.freedesktop.org/software/systemd/man/latest/sd_journal_seek_cursor.html
        sd_journal_next(m_journalHandle);
    }
    else
        sd_journal_seek_head(m_journalHandle);
}

QString JournalLogger::getFieldFromJournal(const QString &fieldName)
{
    const void *data;
    size_t contentLength;
    if (sd_journal_get_data(m_journalHandle, fieldName.toLocal8Bit(), &data, &contentLength) >= 0) {
        // 'Note that the data returned will be prefixed with the field name and "="' [3]
        // [3] https://www.freedesktop.org/software/systemd/man/latest/sd_journal_get_data.htm
        // => +1 for '='
        int offset = fieldName.length() + 1;
        if (contentLength > (size_t)offset) {
            const char* charData = static_cast<const char*>(data);
            QString content = QString::fromUtf8(charData + offset, contentLength - offset);
            removeTrailingControlChars(content);
            return content;
        }
    }
    return QString();
}

QString JournalLogger::getProcessDisplayedFromJournal()
{
    QString syslogIdentifier = getFieldFromJournal("SYSLOG_IDENTIFIER");
    if (!syslogIdentifier.isEmpty())
        return syslogIdentifier;

    QString comm = getFieldFromJournal("_COMM");
    if (!comm.isEmpty())
        return comm;

    QString transport = getFieldFromJournal("_TRANSPORT");
    if (transport == "kernel" || transport == "audit")
        return transport;

    // Debug code to eliminate '???'
    /*const void *data;
    size_t contentLength;
    while (sd_journal_enumerate_data(m_journalHandle, &data, &contentLength)) {
        const char* charData = static_cast<const char*>(data);
        qWarning("%s", charData);
    }*/
    return "???";
}

AbstractJournalLogger::LinePriorities JournalLogger::getPriorityFromJournal()
{
    if (getFieldFromJournal("_TRANSPORT") == "audit")
        return AUDIT;
    QString prioStr = getFieldFromJournal("PRIORITY");
    if (!prioStr.isEmpty())
        return translatePriority(prioStr.toInt());
    return UNKNOWN;
}

AbstractJournalLogger::LinePriorities JournalLogger::translatePriority(int systemPriority)
{
    if (systemPriority < 3)
        return EMERGENCY;
    switch (systemPriority) {
    case 3:
        return ERROR;
    case 4:
        return WARNING;
    case 5:
        return NOTICE;
    case 6:
        return INFO;
    case 7:
        return DEBUG;
    default:
        return UNKNOWN;
    }
}

void JournalLogger::removeTrailingControlChars(QString &line)
{
    while (!line.isEmpty() && line.at(line.size() - 1).unicode() < 32)
        line.chop(1);
}
