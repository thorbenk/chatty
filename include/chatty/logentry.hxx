#ifndef CHATTY__LOGENTRY_HXX
#define CHATTY__LOGENTRY_HXX

#include <boost/date_time/posix_time/posix_time.hpp>

#include <chatty/logutils.hxx>
#include <chatty/archive.hxx>

namespace chatty {

class Logger;

class LogEntry {
    public:
    friend class Logger;
    
    LogEntry();
    LogEntry(int indent);
    
    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;
   
    int indent() const { return indent_; }
    boost::posix_time::ptime constructionTime() { return constructionTime_; }
   
    protected:
        
    Color bgColor_;
    int indent_;
    boost::posix_time::ptime constructionTime_;
    
    private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar  & indent_;
        ar  & constructionTime_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & indent_;
        ar  & constructionTime_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

} /* namespace chatty */

BOOST_CLASS_EXPORT_KEY(chatty::LogEntry)


#endif /* CHATTY__LOGENTRY_HXX */