#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include <chatty/logloop.hxx>
#include <chatty/logger.hxx>

BOOST_CLASS_EXPORT_IMPLEMENT(chatty::LogLoop) 

namespace chatty {

LogLoop::LogLoop()
    : LogEntry(0)
    , loopMax_(0)
    , loopCurrent_(0)
{
}

LogLoop::LogLoop(int indent, std::ostream* o )
    : o_(o)
    , LogEntry(indent)
    , loopMax_(0)
    , loopCurrent_(0)
{
}
    
std::string LogLoop::formatPlaintext() const
{
    return line_;
}

std::string LogLoop::formatTerminalANSI() const
{
    return line_;
}

std::string LogLoop::formatHTML() const
{
    return line_;
}
    
void LogLoop::begin(size_t num)
{
    tic_ = currentTime();
    loopMax_ = num;
    loopCurrent_ = 0;
    operation_ = "";
}

void LogLoop::begin(const std::string& operation, size_t num)
{
    begin(num);
    operation_ = operation;
}

void LogLoop::advance(size_t increment)
{
    loopCurrent_ += increment;
    *o_ << indentStr(indent_);
    if(operation_ != "") {
         *o_ << operation_ << " ";
    }
    double toc = timeDiffSeconds(tic_, currentTime());
    *o_ << "[" << std::setiosflags(std::ios::fixed | std::ios::right) << std::setprecision(3) << std::setw(7)
               << (loopCurrent_)/((double)loopMax_)*100.0 << "%] "
               << std::setw(std::floor(std::log10(loopMax_))+1) << loopCurrent_ << "/" << loopMax_
               << " ETA: " << prettyTime(toc/((double)(loopCurrent_+1))*(loopMax_ - loopCurrent_ -1))
               << "                             \r" << std::flush;
}

void LogLoop::end()
{
    loopCurrent_ = loopMax_-1;
    (*o_) << indentStr(indent_);
    if(operation_ != "") {
        (*o_) << operation_ << " ";
    }
    double toc = timeDiffSeconds(tic_, currentTime());
    (*o_) << "[" << std::setiosflags(std::ios::fixed | std::ios::right) << std::setprecision(3) << std::setw(7)
          << (loopCurrent_+1)/((double)loopMax_)*100.0 << "%] "
          << loopCurrent_+1 << "/" << loopMax_
          << " took " << prettyTime(toc)
          << "                              " << std::endl;
    std::stringstream s;
    if(operation_ != "") {
        s << operation_ << ": ";
    }
    s << "loop over " << loopMax_ << " took " << prettyTime(toc) << std::endl;
    line_ = s.str();
}
    
} /* namespace chatty */