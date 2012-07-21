#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <boost/asio/ip/host_name.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>

#include <chatty/logger.hxx>
#include <chatty/logutils.hxx>
#include <chatty/logtable.hxx>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::Logger)

namespace chatty {
    
Logger* Logger::loggerInstance = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger()
    : LogEntry()
    , o_(&std::cout)
    , hostname_(boost::asio::ip::host_name())
    , hardwareConcurrency_(boost::thread::hardware_concurrency())
{
}

Logger::Logger(std::ostream* o)
    : LogEntry()
    , o_(o)
    , hostname_(boost::asio::ip::host_name())
    , hardwareConcurrency_(boost::thread::hardware_concurrency())
{
}

Logger::Logger(const std::string& filename)
    : LogEntry()
    , o_(&std::cout)
    , hostname_(boost::asio::ip::host_name())
    , hardwareConcurrency_(boost::thread::hardware_concurrency())
{   
    std::ifstream ifs(filename);
    boost::archive::text_iarchive oa(ifs);
    oa >> *this;
    ifs.close();
}

boost::shared_ptr<Logger> Logger::fromCheckpoint(int checkpoint)
{
    boost::shared_ptr<Logger> logger(new Logger);
    for(size_t i=checkpoint; i<log_.size(); ++i) {
        logger->log_.push_back(log_[i]);
    }
    return logger;
}

int Logger::checkpoint() const
{
    return log_.size();
}

boost::shared_ptr<Table> Logger::logTable()
{
    return boost::shared_ptr<Table>(new Table(this) );
}

void Logger::setObjectName(const std::string& name)
{
    objectName_ = name;
}

std::string Logger::objectName() const
{
    return objectName_;
}

void Logger::append(boost::shared_ptr<LogEntry> l)
{
    log_.push_back(l);
    if(fileLog_.get()) {
        *fileLog_ << l->formatPlaintext();
        fileLog_->flush();
    }
}

Logger& Logger::getInstance()
{
    if(loggerInstance == 0) {
        loggerInstance = new Logger();
        loggerInstance->objectName_ = "unnamed Logger instance";
        loggerInstance->indent_ = 0;
    }
    return *loggerInstance;
}

Logger::~Logger()
{
    if(this == loggerInstance) {
        delete loggerInstance;
    }
}

void Logger::setLogfile(const std::string& filepath)
{
    using namespace boost::filesystem;
    path p(filepath);
    bool b = create_directories(basename(p));
    workingPath_ = p.string();

    fileLog_ = boost::make_shared<std::ofstream>(filepath.c_str());
    *fileLog_ << formatPlaintext();
    fileLog_->flush();
}

void Logger::log(const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::warn(const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(red, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::log(Format format, const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(format, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::log(Color fgColor, const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(fgColor, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::log(Format format, Color fgColor, const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(format, fgColor, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::log(Color fgColor, Color bgColor, const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(fgColor, bgColor, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}

void Logger::log(Format format, Color fgColor, Color bgColor, const std::string& log)
{
    boost::shared_ptr<LogLine> l( new LogLine(format, fgColor, bgColor, log, indent_) );
    append(l);
    *o_ << l->formatTerminalANSI();
}


void Logger::logLogger(boost::shared_ptr<Logger> logger)
{
    log_.push_back(logger);
}

void Logger::logTimedStart(const std::string& line)
{
    timedLine_ = boost::shared_ptr<LogTimedLine>( new LogTimedLine(indent_, o_) );
    timedLine_->start(line);
}

void Logger::logTimedStop()
{
    timedLine_->stop();
    append(timedLine_);
    timedLine_.reset();
}

void Logger::logTimedStop(const std::string& text)
{
    timedLine_->stop(text);
    append(timedLine_);
    timedLine_.reset();
}

int Logger::startNewTimer(const std::string& timerName)
{
    int M = 0;
    for(std::map<int, std::pair<std::string, timespec> >::const_iterator it = runningTimers_.begin(); it != runningTimers_.end(); ++it) {
        if(it->first > M) { M = it->first; }
    }
    int newId = M + 1;
    runningTimers_[newId] = std::make_pair(timerName, currentTime());

    std::stringstream s;
    s << "starting timer '" << timerName << "'";
    *o_ << indentStr(indent_) << s.str() << std::endl;
    boost::shared_ptr<LogLine> l( new LogLine(s.str(), indent_) );
    append(l);

    return newId;
}

void Logger::endTimer(int timerId)
{
    std::stringstream s;
    s << "timer '" << runningTimers_[timerId].first << "' took " << prettyTime(timeDiffSeconds(runningTimers_[timerId].second, currentTime()));
    *o_ << indentStr(indent_) << s.str() << std::endl;
    boost::shared_ptr<LogLine> l( new LogLine(s.str(), indent_) );
    append(l);
}

void Logger::beginProgressLoop(size_t num) {
    loop_ = boost::shared_ptr<LogLoop>(new LogLoop(indent_, o_) );
    loop_->begin(num);
}

void Logger::beginProgressLoop(const std::string& operation, size_t num) {
    loop_ = boost::shared_ptr<LogLoop>(new LogLoop(indent_, o_) );
    loop_->begin(operation, num);
}

void Logger::logN(const std::string& s) {
    *o_ << indentStr(indent_) << s << std::flush;
    brokenLine_.push_back(s);
}

void Logger::logFlushline() {
    *o_ << std::endl;
    std::string s;
    for(int i=0; i<brokenLine_.size(); ++i) {
        s += brokenLine_[i];
    }
    brokenLine_.clear();
    boost::shared_ptr<LogLine> l( new LogLine(s, indent_) );
    append(l);
}

void Logger::advanceProgressLoop(size_t increment)
{
    loop_->advance(increment);
}

void Logger::endProgressLoop()
{
    loop_->end();
    append(loop_);
    loop_.reset();
}

void Logger::indent() {
    indent_ += 1;
}
void Logger::unindent() {
    indent_ -= 1;
    if(indent_ < 0) {
        indent_ = 0;
    }
}

void Logger::save(const std::string& filename) const
{
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
    ofs.close();
}

std::string Logger::formatTerminalANSI() const
{
    std::stringstream ss;
    for(size_t i=0; i<log_.size(); ++i) {
        const boost::shared_ptr<LogEntry>& l = log_[i];
        ss << l->formatTerminalANSI(); 
    }
    return ss.str();
}

std::string Logger::formatPlaintext() const
{
    std::stringstream ss;
    for(size_t i=0; i<log_.size(); ++i) {
        const boost::shared_ptr<LogEntry>& l = log_[i];
        ss << l->formatPlaintext();
    }
    return ss.str();
}

std::string Logger::formatHTML() const
{
    int maxIndent = 0;
    for(size_t i=0; i<log_.size(); ++i) {
        const boost::shared_ptr<LogEntry>& l = log_[i];
        maxIndent = std::max(maxIndent, l->indent());
    }
    maxIndent += 1;

    std::stringstream s;
    s << "<table>\n";
        for(size_t i=0; i<log_.size(); ++i) {
            const boost::shared_ptr<LogEntry>& l = log_[i];
            s << indentStr(l->indent()) << "<tr>";

            s << "<td>" << boost::posix_time::to_simple_string(l->constructionTime()) << "</td>" << std::endl; 
            
            for(size_t j=0; j<l->indent(); ++j) {
                s << "<td width=\"20px\">&nbsp</td>";
            }

            s << "<td colspan=\"" << maxIndent - l->indent() << "\" style=\"bgcolor:" << htmlBgColor(l->bgColor_) << ";\">";
            s << l->formatHTML();
            s << "</td></tr>";
            s << std::endl;

    }
    s << "</table>\n";
    return s.str();
}

} // namespace chatty