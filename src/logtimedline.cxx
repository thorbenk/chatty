#include <iostream>
#include <sstream>

#include <chatty/logtimedline.hxx>
#include <chatty/logutils.hxx>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::LogTimedLine) 

namespace chatty {

LogTimedLine::LogTimedLine()
    : LogEntry(0)
    , o_(&std::cout)
{
}

LogTimedLine::LogTimedLine(int indent, std::ostream* o)
    : LogEntry(indent)
    , o_(o)
{
}

std::string LogTimedLine::formatPlaintext() const
{
    return line_;
}

std::string LogTimedLine::formatTerminalANSI() const 
{
    return line_;
}

std::string LogTimedLine::formatHTML() const
{
    return line_;
}
    
void LogTimedLine::start(const std::string& line)
{
    operation_ = line;
    *o_ << indentStr(indent_) << line << std::flush;
    tic_ = currentTime();
}

void LogTimedLine::stop()
{
    double toc = timeDiffSeconds(tic_, currentTime());
    *o_ << " ... " << prettyTime(toc) << std::endl;
    std::stringstream s;
    s << operation_ << " ... " << prettyTime(toc) << std::endl;
    line_ = s.str();
}

void LogTimedLine::stop(const std::string& text)
{
    double toc = timeDiffSeconds(tic_, currentTime());
    *o_ << ": " << text;
    *o_ << " ... " << prettyTime(toc) << std::endl;
    std::stringstream s;
    s << ": " << text;
    s << operation_ << " ... " << prettyTime(toc) << std::endl;
    line_ = s.str();
}

} /* namespace chatty */
