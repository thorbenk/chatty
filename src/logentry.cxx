#include <boost/date_time/posix_time/ptime.hpp>

#include <chatty/logutils.hxx>
#include <chatty/logentry.hxx>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::LogEntry) 

namespace chatty {

LogEntry::LogEntry()
    : indent_(0)
    , constructionTime_(boost::posix_time::microsec_clock::local_time())
    , bgColor_(none)
{
}

LogEntry::LogEntry(int indent)
    : indent_(indent)
    , constructionTime_(boost::posix_time::microsec_clock::local_time())
    , bgColor_(none)
{
}

std::string LogEntry::formatPlaintext()    const { return ""; }
std::string LogEntry::formatTerminalANSI() const { return ""; }
std::string LogEntry::formatHTML()         const { return ""; }

} /* namespace chatty */
