#include "abstractjournallogger.h"

Q_DECLARE_METATYPE(AbstractJournalLogger::LinePriorities)

AbstractJournalLogger::AbstractJournalLogger()
{
    static bool registered = false;
    if (!registered)
        qRegisterMetaType<LinePriorities>();
}

QString AbstractJournalLogger::getPrioText(LinePriorities prio)
{
    switch(prio) {
    case EMERGENCY:
        return "<emergency>";
        break;
    case ERROR:
        return "<error>";
        break;
    case WARNING:
        return "<warning>";
        break;
    case NOTICE:
        return "<notice>";
        break;
    case INFO:
        return "<info>";
        break;
    case DEBUG:
        return "<debug>";
        break;
    case AUDIT:
        return "<audit>";
        break;
    default:
        return "<unkown>";
    }
}
