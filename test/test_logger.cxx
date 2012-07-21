#include <list>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/regex.hpp>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define private public
#define protected public
#include <chatty/logger.hxx>
#undef protected
#undef private
#include <chatty/logtable.hxx>

using namespace chatty;

std::string unescape(const std::string& str){
    std::string res;
    for(int i=0; i<str.length(); ++i){
        switch (str[i]){
            case '\r':
                res+="\\r\n";
                break;
            case '\n':
                res+="\\n\n";
                break;
            case '\\':
                res+="\\\\";
                break;
            case '\t':
                res+="\\t";
                break;
            case '\x1b':
                res+="\\x1b";
            default:
                res += str[i];
        }
    }
    return res;
}

void saveLogger(const Logger& l, const std::string& filename)
{
    std::string p = (boost::filesystem::temp_directory_path()/filename).string();
    l.save(p);
}

std::string loadLogger(const std::string& filename)
{
    
    Logger l2((boost::filesystem::temp_directory_path()/filename).string());
    return l2.formatTerminalANSI();
}

void saveHTML(const Logger& l, const std::string& filename)
{
    std::string p = (boost::filesystem::temp_directory_path()/filename).string();
    std::string s = l.formatHTML();
    std::ofstream ofs(p);
    ofs << s;
    ofs.close();
}

BOOST_AUTO_TEST_CASE(test_log_plain)
{
    std::stringstream A; Logger l(&A);
    
    l.log("Hello, World!");
    l.log("Hello, Chatty!");
    
    std::stringstream B;
    B << "Hello, World!" << std::endl
      << "Hello, Chatty!" << std::endl;
    
    BOOST_CHECK_EQUAL(A.str(), B.str());
   
    auto f = "chatty__test_log_plain.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_loglogger)
{
    Logger l;
    l.setObjectName("l");
    
    l.log("Hello, World!");
    int c = l.checkpoint();
    l.log("Hello, Chatty!");
    auto l2 = l.fromCheckpoint(c);
    l2->setObjectName("l2");
    l.logLogger(l2); 
    
    std::string A = l.formatTerminalANSI();
    
    std::stringstream B;
    B << "Hello, World!" << std::endl
      << "Hello, Chatty!" << std::endl
      << "Hello, Chatty!" << std::endl;
    
    BOOST_CHECK_EQUAL(A, B.str());
   
    auto f = "chatty__test_loglogger.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_starred_line)
{
    std::stringstream A; Logger l(&A);
    
    l.log(StarredLine, "Starred Line");
    l.indent();
    l.log(StarredLine, "Starred Line");
    l.indent();
    l.log(StarredLine, "Starred Line");
    l.unindent();
    l.unindent();
    std::stringstream B;
    B << "* Starred Line" << std::endl;
    B << "  * Starred Line" << std::endl;
    B << "    * Starred Line" << std::endl;
    BOOST_CHECK_EQUAL(A.str(), B.str());
    
    auto f = "chatty__test_starred_line.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_header)
{
    std::stringstream A; Logger l(&A);
    
    l.log(Header1, "Header 1!");
    l.indent();
    l.log(Header1, "Header 1!");
    l.unindent();
    l.log(Header2, "Header 2!");
    l.indent();
    l.log(Header2, "Header 2!");
    l.unindent();
    l.log(Header3, "Header 3!");
    l.indent();
    l.log(Header3, "Header 3!");
    l.unindent();
    std::stringstream B;
    B << "*******************************************************************************" << std::endl
      << "Header 1!" << std::endl
      << "*******************************************************************************" << std::endl;
    B << "  *****************************************************************************" << std::endl
      << "  Header 1!" << std::endl
      << "  *****************************************************************************" << std::endl;
      
    B << "Header 2!" << std::endl
      << "===============================================================================" << std::endl;
    B << "  Header 2!" << std::endl
      << "  =============================================================================" << std::endl;
      
    B << "Header 3!" << std::endl
      << "-------------------------------------------------------------------------------" << std::endl;
    B << "  Header 3!" << std::endl
      << "  -----------------------------------------------------------------------------" << std::endl;
    BOOST_CHECK_EQUAL(A.str(), B.str());
    
    //save logger and open it again
    auto f = "chatty__test_header.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_log_color)
{
    std::stringstream A; Logger l(&A);
    
    l.log(chatty::green, "Hello, World!");
    std::stringstream B; B << "\x1b[0m" << "\x1b[032m" << "Hello, World!" << std::endl << "\x1b[0m";
    BOOST_CHECK_EQUAL(A.str(), B.str());
    
    //save logger and open it again
    auto f = "chatty__test_color.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_table1)
{
    std::stringstream A; Logger l(&A);
    
    auto t = l.logTable(); 
    t->header() << "One" << "Two" << "Three" << std::endl;
    t->row() << "aaaa" << "b" << "ccccc" << std::endl;
    t->row() << "dd" << "eeeeee" << "f" << std::endl;
    t->end();
    std::stringstream B;
    B << "One |Two   |Three|" << std::endl
      << "----|------|-----|" << std::endl
      << "aaaa|b     |ccccc|" << std::endl
      << "dd  |eeeeee|f    |" << std::endl;
    BOOST_CHECK_EQUAL(A.str(), B.str());

    std::string f = "chatty__test_table1";
    
    saveHTML(l, f+".html");
    
    saveLogger(l, f+".txt" );
    auto savedLog = loadLogger(f+".txt");
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_table2)
{
    std::stringstream A; Logger l(&A);
    
    auto t2 = l.logTable();
    t2->row() << "1" << "2" << "3" << std::endl;
    t2->row() << 1 << 2 << 3 << std::endl;
    t2->row() << 1.234 << 2.5 << 708.9 << std::endl;
    t2->end();
    std::stringstream B;
    B << "1    |2  |3    |" << std::endl
      << "1    |2  |3    |" << std::endl
      << "1.234|2.5|708.9|" << std::endl;
    BOOST_CHECK_EQUAL(A.str(), B.str());
    
    auto f = "chatty__test_table2.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_indent)
{
    std::stringstream A; Logger l(&A);

    l.log("logging a line");
    l << "test " << 5 << " is a number" << std::endl;
    l.indent();
    l << "indented line: " << 100 << " is a number" << std::endl;
    l.unindent();
    l << "back to normal" << std::endl;    
    
    std::stringstream B;
    B << "logging a line" << std::endl
      << "test 5 is a number" << std::endl
      << "  indented line: 100 is a number" << std::endl
      << "back to normal" << std::endl;
    BOOST_CHECK_EQUAL(A.str(), B.str());
    
    auto f = "chatty__test_indent.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(unescape(savedLog), unescape(B.str()));
}

BOOST_AUTO_TEST_CASE(test_timer) {
    std::stringstream A; Logger l(&A);
    int timer = l.startNewTimer("mytimer");
    l.log("some line");
    l.endTimer(timer);
   
    std::stringstream B_ss;
    B_ss << "starting timer 'mytimer'" << std::endl
         << "some line" << std::endl
         << "timer 'mytimer' took NUMBERs" << std::endl;
    std::string B = B_ss.str();
   
    //match a number (also scientific format)
    boost::regex pattern("[-+]?\\d+\\.\\d+([eE][-+]?\\d+)?");
    std::string A_repl = boost::regex_replace(A.str(), pattern, "NUMBER");
    
    BOOST_CHECK_EQUAL(A_repl, B);
    
    auto f = "chatty__test_timer.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(boost::regex_replace(savedLog, pattern, "NUMBER"), B); 
}

BOOST_AUTO_TEST_CASE(test_timedline) {
    std::stringstream A; Logger l(&A);
   
    l.logTimedStart("test log timed line");
    l.logTimedStop();
    
    std::stringstream B_ss;
    B_ss << "test log timed line ... NUMBERs" << std::endl;
    std::string B = B_ss.str();
   
    //match a number (also scientific format)
    boost::regex pattern("[-+]?\\d+\\.\\d+([eE][-+]?\\d+)?");
    std::string A_repl = boost::regex_replace(A.str(), pattern, "NUMBER");
    
    BOOST_CHECK_EQUAL(A_repl, B);
    
    auto f = "chatty__test_timedline.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    BOOST_CHECK_EQUAL(boost::regex_replace(savedLog, pattern, "NUMBER"), B); 
}

BOOST_AUTO_TEST_CASE(test_loop) {
    std::stringstream A_ss; Logger l(&A_ss);
    
    l.beginProgressLoop("my loop", 2);
    for(int i=0; i<2; ++i) {
        l.advanceProgressLoop(1);
    }
    l.endProgressLoop();
    
    std::string A = unescape(A_ss.str());
   
    std::stringstream B_ss;
    B_ss << "my loop [ 50.000%] 1/2 ETA: NUMBER\\r" << std::endl
         << "my loop [100.000%] 2/2 ETA: NUMBER\\r" << std::endl
         << "my loop [100.000%] 2/2 took NUMBER\\n" << std::endl;
    std::string B = B_ss.str();
   
    //match a number (also scientific format) [number]s<space>
    boost::regex pattern("[-+]?\\d+(\\.\\d+([eE][-+]?\\d+)?)?s[[:space:]]*");
    std::string A_repl = boost::regex_replace(A, pattern, "NUMBER");
    
    BOOST_CHECK_EQUAL(A_repl, B);
    
    //save logger and open it again
    auto f = "chatty__test_loop.txt";
    saveLogger(l, f);
    auto savedLog = loadLogger(f);
    std::stringstream loopDone; loopDone << "my loop: loop over 2 took NUMBER";
    BOOST_CHECK_EQUAL(boost::regex_replace(savedLog, pattern, "NUMBER"), loopDone.str()); 
}
