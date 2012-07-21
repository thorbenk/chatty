#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <chatty/logutils.hxx>
#include <chatty/logtable.hxx>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::Table) 
BOOST_CLASS_EXPORT_IMPLEMENT(chatty::Table::Row) 

namespace chatty {
    
Table::Table(Logger* l, const std::string& format)
    : LogEntry(l->indent_)
    , numColumns_(-1)
    , format_(format)
    , logger_(l)
{
    if(format.size() > 0) {
        numColumns_ = format.size();
    }
}

void Table::end()
{
    logger_->append(shared_from_this());
    *(logger_->o_) << formatTerminalANSI();
}

Table::Row& Table::header()
{
    boost::shared_ptr<Row> row(new Row(shared_from_this()));
    row->setHeader(true);
    rows_.push_back(boost::shared_ptr<Row>(row));
    return *rows_.back();
}

Table::Row& Table::row()
{
    rows_.push_back(boost::shared_ptr<Row>(new Row(shared_from_this())));
    return *rows_.back();
}


std::string Table::formatPlaintext() const
{
    return formatTerminalANSI();
}

std::string Table::formatTerminalANSI() const
{
    std::stringstream s;
    std::vector<int> colMax(numColumns(), -1);
    for(auto row = rows_.begin(); row!=rows_.end(); ++row) {
        int c = 0;
        for(auto col = (**row).row_.begin(); col!=(**row).row_.end(); ++col, ++c) {
            if(colMax[c] < (int)col->size()) {
                int s = col->size();
                colMax[c] = s;
            }
        }
    }

    for(auto row = rows_.begin(); row!=rows_.end(); ++row) {
        s << indentStr(logger_->indent_);
        int c = 0;
        for(auto col = (**row).row_.begin(); col!=(**row).row_.end(); ++col, ++c) {
            if(format_[c] == 'l') {
                s << std::left;
            }
            else if(format_[c] == 'r') {
                s << std::right;
            }
            s << std::setw(colMax[c]) << *col << "|";
        }
        s << std::endl;
        if( (*row)->header_ ) {
            int c = 0;
            for(auto col = (**row).row_.begin(); col!=(**row).row_.end(); ++col, ++c) {
                s << std::string(colMax[c], '-') << '|';
            }
            s << std::endl;
        }
    }
    return s.str();
}

std::string Table::formatHTML() const
{
    std::stringstream s;
    std::vector<int> colMax(numColumns(), -1);
    for(auto row = rows_.begin(); row!=rows_.end(); ++row) {
        int c = 0;
        for(auto col = (**row).row_.begin(); col!=(**row).row_.end(); ++col, ++c) {
            if(colMax[c] < (int)col->size()) {
                int s = col->size();
                colMax[c] = s;
            }
        }
    }

    s << "<table border=\"1\">" << std::endl;
    for(auto row = rows_.begin(); row!=rows_.end(); ++row) {
        s << "<tr>" << std::endl;
        int c = 0;
        for(auto col = (**row).row_.begin(); col!=(**row).row_.end(); ++col, ++c) {
            if( (*row)->header_ ) {
                s << "<th ";
            }
            else {
                s << "<td ";
            }
            if(format_[c] == 'l') {
                s << "align=\"left\">";
            }
            else if(format_[c] == 'r') {
                s << "align=\"right\">";
            }
            else {
                s << ">";
            }
            s << std::endl;
            s << *col;
            if( (*row)->header_ ) {
                s << "</th>";
            }
            else {
                s << "</td>";
            }
            s << std::endl;
        }
        s << "</tr>" << std::endl; 
    }
    s << "</table>" << std::endl; 
    return s.str();
}

int Table::numColumns() const {
    return numColumns_;
}

Table::Row::Row(boost::shared_ptr<Table> t)
    : table_(t)
    , header_(false)
{
}

void Table::Row::finish()
{
    if(table_->numColumns_ > 0) {
        if(row_.size() != table_->numColumns_) {
            std::stringstream err;
            err << "previously " << table_->numColumns_ << " columns"
                << ", now " << row_.size();
            throw std::runtime_error(err.str());
        }
    }
    else {
        table_->format_ = std::string(row_.size(), 'l');
    }
    table_->numColumns_ = row_.size();
}
void Table::Row::setHeader(bool header) {
    header_ = header;
}

Table::Row& operator<<(Table::Row& r, std::ostream& (*o) (std::ostream&))
{
    r.finish();
    return r;
}

Table::Row& operator<<(Table::Row& r, const std::string& str)
{
    r.row_.push_back(str);
    return r;
}

} /* namespace chatty */