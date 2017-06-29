#include <chatty/logutils.hxx>
#include <chatty/logline.hxx>

#include <sstream>
#include <iostream>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::LogLine) 

namespace chatty {

LogLine::LogLine() : LogEntry(0), line_(""), fgColor_(none), format_(NoFormat) {}
LogLine::LogLine(const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(none), format_(NoFormat) {}
LogLine::LogLine(Format fmt, const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(none), format_(fmt) {}
LogLine::LogLine(Color f, const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(f), format_(NoFormat) {}
LogLine::LogLine(Format fmt, Color f, const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(f), format_(fmt) {}
LogLine::LogLine(Color f, Color b, const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(f), format_(NoFormat) { bgColor_ = b; }
LogLine::LogLine(Format fmt, Color f, Color b, const std::string& l, int i) : LogEntry(i), line_(l), fgColor_(f), format_(fmt) { bgColor_ = b; }

LogLine::~LogLine() { }

LogLine::LogLine(const LogLine& l) :
    LogEntry(l.indent_)
  , line_(l.line_)
  , fgColor_(l.fgColor_)
  , format_(l.format_)
{
}

LogLine& LogLine::operator=(const LogLine& l)
{
    if(this != &l) {
        line_    = l.line_;
        fgColor_ = l.fgColor_;
        format_  = l.format_;
    }
    return *this;
}

std::string LogLine::formatPlaintext() const
{
    std::stringstream s;

    if(format_ == Header1) {
        s << indentStr(indent_) << std::string(std::max(0, 79 - INDENT_WIDTH*indent_), '*') << std::endl;
    }
    if(format_ == StarredLine) {
        s << indentStr(indent_) << "* ";
    }
    s << indentStr(indent_) << line_ << std::endl;
    if(format_ == Header1) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '*') << std::endl;
    }
    else if(format_ == Header2) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '=') << std::endl;
    }
    else if(format_ == Header3) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '-') << std::endl;
    }
    return s.str();
}

std::string LogLine::formatTerminalANSI() const
{
    std::stringstream s;
   
    if(bgColor_ != none || fgColor_ != none) {
        s << ansiColorReset << ansiBgColor(bgColor_) << ansiFgColor(fgColor_);
    }
    
    if(format_ == Header1) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '*') << std::endl;
    }
    if(format_ == StarredLine) {
        s << indentStr(indent_) << "* ";
    }
    else {
        s << indentStr(indent_);
    }
    
    s << line_ << std::endl;
    
    if(format_ == Header1) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '*') << std::endl;
    }
    else if(format_ == Header2) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '=') << std::endl;
    }
    else if(format_ == Header3) {
        s << indentStr(indent_) << std::string(std::max(79 - INDENT_WIDTH*indent_, 0), '-') << std::endl;
    }
    
    if(bgColor_ != none || fgColor_ != none) {
        s << ansiColorReset;
    }
    return s.str();
}

std::string LogLine::formatHTML() const
{
    std::stringstream s;
    if(format_ == Header1) s << "<h1>";
    else if(format_ == Header2) s << "<h2>";
    else if(format_ == Header3) s << "<h3>";
    s << "<span style=\"color:" << htmlFgColor(fgColor_) << "\">";
    s << line_ << "</span>";
    if(format_ == Header1) s << "</h1>";
    else if(format_ == Header2) s << "</h2>";
    else if(format_ == Header3) s << "</h3>";
    return s.str();
}

} /* namespace chatty */
