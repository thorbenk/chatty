#include <sstream>
#include <iostream>

#include <chatty/logutils.hxx>

namespace chatty {

const char* ansiBgColor(Color c)
{
    switch(c) {
        case red:     return "\x1b[041m";
        case green:   return "\x1b[042m";
        case yellow:  return "\x1b[043m";
        case blue:    return "\x1b[044m";
        case magenta: return "\x1b[045m";
        case cyan:    return "\x1b[046m";
        case none:    return "";
    }
}

const char* htmlBgColor(Color c)
{
    switch(c) {
        case none:    return "#ffffff";
        case red:     return "#ff0000";
        case green:   return "#00ff00";
        case yellow:  return "#ffff00";
        case blue:    return "#0000ff";
        case magenta: return "#ff00ff";
        case cyan:    return "#00ffff";
    }
}

const char* ansiFgColor(Color c)
{
    switch(c) {
        case none:    return "";
        case red:     return "\x1b[031m";
        case green:   return "\x1b[032m";
        case yellow:  return "\x1b[033m";
        case blue:    return "\x1b[034m";
        case magenta: return "\x1b[035m";
        case cyan:    return "\x1b[036m";
    }
}

const char* htmlFgColor(Color c)
{
    switch(c) {
        case none:    return "#000000";
        default:      return htmlBgColor(c);
    }
}

std::string indentStr(int indent)
{
    return std::string(std::max(INDENT_WIDTH*indent, 0), ' ');
}

std::string prettyTime(double sec)
{
    double rest    = sec;

    int days       = (int)rest / (60*60*24);
    rest           = rest - days*(60*60*24);

    int hours      = (int)rest / (60*60);
    rest           = rest - hours*(60.60);

    int minutes    = (int)rest / 60;
    rest           = rest - minutes*60;

    double seconds = rest;

    std::stringstream s;
    if(days     > 0) s << days << "d ";
    if(hours    > 0) s << hours << "h ";
    if(minutes  > 0) s << minutes << "m ";
    s << seconds << "s";
    return s.str();
}

} /* namespace chatty */