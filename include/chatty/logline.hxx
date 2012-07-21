#ifndef CHATTY__LOGLINE_HXX
#define CHATTY__LOGLINE_HXX

#include <chatty/logutils.hxx>
#include <chatty/logentry.hxx>
#include <chatty/archive.hxx>

namespace chatty {

class LogLine : public LogEntry {
    public:
    LogLine();
    LogLine(const std::string& l, int i);
    LogLine(Format fmt, const std::string& l, int i);
    LogLine(Color f, const std::string& l, int i);
    LogLine(Format fmt, Color f, const std::string& l, int i);
    LogLine(Color f, Color b, const std::string& l, int i);
    LogLine(Format fmt, Color f, Color b, const std::string& l, int i);
    
    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;

    //handle the void* pointer explicitly here!
    LogLine(const LogLine& l);
    LogLine& operator=(const LogLine& l);
    ~LogLine();

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & line_;
        ar & fgColor_;
        ar & format_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & line_;
        ar & fgColor_;
        ar & format_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    private:
    friend class boost::serialization::access;
    
    std::string line_;
    Color fgColor_;
    Format format_;
    
};

} /* namespace chatty */

BOOST_CLASS_EXPORT_KEY(chatty::LogLine)

#endif /*CHATTY__LOGLINE_HXX*/
