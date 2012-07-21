#ifndef CHATTY__LOGUTILS_HXX
#define CHATTY__LOGUTILS_HXX

#include <string>
#include <time.h>

#define INDENT_WIDTH 2

namespace chatty {
    enum Color {
        none = 0,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan
    };

    enum Format {
        NoFormat = 0,
        Header1,
        Header2,
        Header3,
        StarredLine
    };

    const char* ansiFgColor(Color c);
    const char* ansiBgColor(Color c);
    static const char ansiColorReset[] = "\x1b[0m";
        
    const char* htmlFgColor(Color c);
    const char* htmlBgColor(Color c);

    std::string indentStr(int indent);
    
    inline double timeDiffSeconds(timespec tic, timespec toc) {
        return (((double)toc.tv_sec + toc.tv_nsec*1.0E-9) -
                ((double)tic.tv_sec + tic.tv_nsec*1.0E-9));
    }
    inline timespec currentTime() {
        timespec t;
        clock_gettime(CLOCK_REALTIME, &t);
        return t;
    }
    
    std::string prettyTime(double sec);

} /* namespae chatty */

#endif /*CHATTY__LOGUTILS_HXX*/