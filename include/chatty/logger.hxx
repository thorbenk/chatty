#ifndef CHATTY__LOGGER_HXX
#define CHATTY__LOGGER_HXX

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include <chatty/archive.hxx>
#include <chatty/logline.hxx>
#include <chatty/logloop.hxx>
#include <chatty/logtimedline.hxx>
#include <chatty/logentry.hxx>

namespace chatty {

class Logger;

template<class T>
Logger& operator<< (Logger& log, const T& t);
Logger& operator<< (Logger& log, std::ostream& (*o) (std::ostream&));

class Table;
class LogLoop;

class Logger : public LogEntry {
    public:
    
    Logger(const std::string& filename);
    ~Logger();
    
    static Logger& getInstance();
    
    void setObjectName(const std::string& name);
    std::string objectName() const;

    void setLogfile(const std::string& filepath);

    boost::shared_ptr<Logger> fromCheckpoint(int checkpoint);
    int checkpoint() const;
    
    //simple logging 
    void log(const std::string&);
    void log(Format format, const std::string&);
    void log(Color fgColor, const std::string&);
    void log(Format format, Color fgColor, const std::string&);
    void log(Color fgColor, Color bgColor, const std::string&);
    void log(Format format, Color fgColor, Color bgColor, const std::string&);
    void warn(const std::string&);
    void logN(const std::string& s);
    void logFlushline();
    void logLogger(boost::shared_ptr<Logger> logger);

    //indentation
    void indent();
    void unindent();
    
    //table
    boost::shared_ptr<Table> logTable();

    //timer
    int startNewTimer(const std::string& timerName);
    void endTimer(int timerId);
    void logTimedStart(const std::string& line);
    void logTimedStop();
    void logTimedStop(const std::string& text);

    //loop progress 
    boost::shared_ptr<LogLoop> progressLoop(); 
    void beginProgressLoop(size_t num);
    void beginProgressLoop(const std::string& operation, size_t num);
    void advanceProgressLoop(size_t increment);
    void endProgressLoop();

    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;
    
    //save
    void save(const std::string& filename) const;

    private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & objectName_;
        ar & hostname_;
        ar & log_;
        ar & workingPath_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & objectName_;
        ar & hostname_;
        ar & log_;
        ar & workingPath_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    static Logger* loggerInstance;
    
    void append(boost::shared_ptr<LogEntry> l);

    template<class T>
    friend Logger& operator<< (Logger& log, const T& t) {
        std::stringstream s; s << t;
        log.brokenLine_.push_back(s.str());
        return log;
    }
    friend Logger& operator<< (Logger& log, std::ostream& (*o) (std::ostream&)) {
        std::stringstream s;
        for(size_t i=0; i<log.brokenLine_.size(); ++i) {
            s << log.brokenLine_[i];
        }
        log.log(s.str());
        log.brokenLine_.clear();
        return log;
    }

    std::string objectName_;
    std::string hostname_;
    unsigned hardwareConcurrency_;
    
    std::vector< boost::shared_ptr<LogEntry> > log_;
    
    std::ostream* o_;
    
    std::vector<std::string> brokenLine_;
    boost::shared_ptr< LogTimedLine > timedLine_;
    boost::shared_ptr< LogLoop      > loop_;
    
    std::map<int, std::pair<std::string, timespec> > runningTimers_;

    boost::shared_ptr<std::ofstream> fileLog_;
    std::string workingPath_;

    friend class Table;
    
    Logger();
    Logger(std::ostream* o);
    Logger(const Logger&);
    Logger& operator=(const Logger&);
    
}; // class Logger

} // namespace chatty

BOOST_CLASS_EXPORT_KEY(chatty::Logger);

#endif /* CHATTY__LOGGER_HXX */
