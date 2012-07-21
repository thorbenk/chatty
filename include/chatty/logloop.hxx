#ifndef CHATTY__LOGLOOP_HXX
#define CHATTY__LOGLOOP_HXX

#include <chatty/logentry.hxx>
#include <chatty/archive.hxx>

namespace chatty {

class LogLoop : public LogEntry {
    public:
    LogLoop();
    LogLoop(int indent, std::ostream* o );
    
    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;
    
    void begin(size_t num);
    void begin(const std::string& operation, size_t num);
    void advance(size_t increment);
    void end();

    private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar  & boost::serialization::base_object<LogEntry>(*this);
        ar  & operation_;
        ar  & loopMax_;
        ar  & loopCurrent_;
        ar  & line_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        
        ar  & boost::serialization::base_object<LogEntry>(*this);
        ar  & operation_;
        ar  & loopMax_;
        ar  & loopCurrent_;
        ar  & line_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    std::string operation_;
    long loopMax_;
    long loopCurrent_;
    timespec tic_;
    std::string line_;
    std::ostream* o_;
    
};

} /* namespace chatty */

BOOST_CLASS_EXPORT_KEY(chatty::LogLoop)

#endif /* CHATTY__LOGLOOP_HXX */