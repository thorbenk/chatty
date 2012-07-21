#ifndef CHATTY__LOGTIMELINE_HXX
#define CHATTY__LOGTIMELINE_HXX

#include <chatty/logentry.hxx>
#include <chatty/archive.hxx>

namespace chatty {

class LogTimedLine : public LogEntry {
    public:
    LogTimedLine();
    LogTimedLine(int indent, std::ostream* o);
    
    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;
 
    void start(const std::string& line);
    void stop();
    void stop(const std::string& text);
    
    private:
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar  & boost::serialization::base_object<LogEntry>(*this);
        ar  & operation_;
        ar  & line_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & boost::serialization::base_object<LogEntry>(*this);
        ar  & operation_;
        ar  & line_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
       
    std::ostream* o_;
    std::string operation_;
    timespec tic_;
    std::string line_;
};

} /* namespace chatty */

BOOST_CLASS_EXPORT_KEY(chatty::LogTimedLine)

#endif /* CHATTY__LOGTIMELINE_HXX */