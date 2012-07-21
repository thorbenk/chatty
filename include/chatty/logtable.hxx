#ifndef CHATTY__LOGTABLE_HXX
#define CHATTY__LOGTABLE_HXX

#include <vector>
#include <sstream>

#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <chatty/logutils.hxx>
#include <chatty/logentry.hxx>
#include <chatty/logger.hxx>


namespace chatty {
    
class Table : public boost::enable_shared_from_this<Table>, public LogEntry {
    public:

    class Row {
        public:
        Row() {}
        Row(boost::shared_ptr<Table> t);
        void finish();
        void setHeader(bool header);

        private:
        boost::shared_ptr<Table> table_;
        std::vector<std::string> row_;
        bool                     header_;

        friend Row& operator<<(Row& r, const std::string& str);
        template<class T>
        friend Row& operator<<(Row& r, const T& t);
        friend Row& operator<<(Row& r, std::ostream& (*o) (std::ostream&));
        friend class Table;
        
        friend class boost::serialization::access;
   
        template<class Archive>
        void save(Archive & ar, const unsigned int version) const
        {
            ar & table_;
            ar & row_;
            ar & header_;
        }
        template<class Archive>
        void load(Archive & ar, const unsigned int version)
        {
            ar & table_;
            ar & row_;
            ar & header_;
        }
        BOOST_SERIALIZATION_SPLIT_MEMBER()
    };

    Table() {}
    Table(Logger* l, const std::string& format = "");

    Row& header();
    Row& row();
    
    void end();

    int numColumns() const;

    virtual std::string formatPlaintext() const;
    virtual std::string formatTerminalANSI() const;
    virtual std::string formatHTML() const;
    
    friend Row& operator<<(Row& r, std::ostream& (*o) (std::ostream&));
    friend Row& operator<<(Row& r, const std::string& str);
    template<class T>
    friend Row& operator<<(Row& r, const T& t) {
        std::stringstream s;
        s << t;
        r << s.str();
        return r;
    }
    
    private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & rows_;
        ar & numColumns_;
        ar & format_;
        ar & logger_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        
        ar & boost::serialization::base_object<LogEntry>(*this);
        ar & rows_;
        ar & numColumns_;
        ar & format_;
        ar & logger_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    std::vector<boost::shared_ptr<Row> > rows_;
    int                                  numColumns_;
    std::string                          format_;
    boost::shared_ptr<Logger>            logger_;
};

} /* namespace chatty */

BOOST_CLASS_EXPORT_KEY(chatty::Table)
BOOST_CLASS_EXPORT_KEY(chatty::Table::Row)

#endif /* CHATTY__LOGTABLE_HXX */
