#include <chatty/logger.hxx>

int main(int argc, char** argv)
{
    chatty::Logger& l = chatty::Logger::getInstance();
    l.log("line 1"); 
    l.log("line 2"); 
    l.log("line 3"); 
    auto c = l.checkpoint();
    l.log("line 4");
    l.log("line 5");
    auto x = l.fromCheckpoint(c);
   
    /*
    l.cd("/tmp/loggertest");
    
    l.logTimedStart("test log timed line");
    l.logTimedStop();

    std::ofstream html("/tmp/testlogger.html");
    html << l.backlogHTML();
    html.close();
    
    std::ofstream ofs("/tmp/testserialize.txt");
    boost::archive::text_oarchive oa(ofs);
    oa << l;
    ofs.close();
   
    std::cout << "xxxx" << std::endl;
    chatty::Logger l2("/tmp/testserialize.txt");
    l2.backlog(std::cout);
    */
}
