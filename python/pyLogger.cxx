#include "Python.h"

#include <chatty/logger.hxx>
#include <boost/python.hpp>

using namespace boost::python;
using namespace chatty;

namespace PyLogger {
    class TimedLineContextManager {
        public:
        TimedLineContextManager(const std::string& line) : line_(line) {}
        Logger& enter() { Logger::getInstance().logTimedStart(line_); return Logger::getInstance(); }
        void exit(object type, object value, object traceback) { Logger::getInstance().logTimedStop(); }
        private:
        std::string line_;
    };

    class IndentationContextManager {
        public:
        IndentationContextManager() {}
        Logger& enter() { Logger::getInstance().indent(); return Logger::getInstance(); }
        void exit(object type, object value, object traceback) { Logger::getInstance().unindent(); }
    };

    class ProgressLoopContextManager {
        public:
        ProgressLoopContextManager(const std::string line, size_t num) : line_(line), num_(num) {}
        Logger& enter() { Logger::getInstance().beginProgressLoop(line_, num_); return Logger::getInstance(); }
        void exit(object type, object value, object traceback) { Logger::getInstance().endProgressLoop(); }
        private:
        std::string line_;
        size_t num_;
    };

    TimedLineContextManager logTimed(Logger& logger, const std::string& line) {
        return TimedLineContextManager(line);
    }

    IndentationContextManager indentation(Logger& logger) {
        return IndentationContextManager();
    }

    ProgressLoopContextManager progressLoop_num(Logger& logger, size_t num) {
        return ProgressLoopContextManager("", num);
    }
    ProgressLoopContextManager progressLoop_str_num(Logger& logger, const std::string& line, size_t num) {
        return ProgressLoopContextManager(line, num);
    }
}

BOOST_PYTHON_MODULE_INIT(_chatty) {

    class_<PyLogger::TimedLineContextManager>("TimedLineContextManager", init<std::string>())
        .def("__enter__", &PyLogger::TimedLineContextManager::enter, return_value_policy<reference_existing_object>())
        .def("__exit__",  &PyLogger::TimedLineContextManager::exit)
    ;
    class_<PyLogger::IndentationContextManager>("IndentationContextManager")
        .def("__enter__", &PyLogger::IndentationContextManager::enter, return_value_policy<reference_existing_object>())
        .def("__exit__",  &PyLogger::IndentationContextManager::exit)
    ;
    class_<PyLogger::ProgressLoopContextManager>("ProgressLoopContextManager", init<std::string, size_t>())
        .def("__enter__", &PyLogger::ProgressLoopContextManager::enter, return_value_policy<reference_existing_object>())
        .def("__exit__",  &PyLogger::ProgressLoopContextManager::exit)
    ;

    enum_<Color>("Color")
        .value("none",    none)
        .value("red",     red)
        .value("green",   green)
        .value("yellow",  yellow)
        .value("blue",    blue)
        .value("magenta", magenta)
        .value("cyan",    cyan)
    ;

    enum_<Format>("Format")
        .value("none",        NoFormat)
        .value("header1",     Header1)
        .value("header2",     Header2)
        .value("header3",     Header3)
        .value("starredLine", StarredLine)
    ;

    void (Logger::*log_str)(const std::string&) = &Logger::log;
    void (Logger::*log_fmt_str)(Format format, const std::string&) = &Logger::log;
    void (Logger::*log_col_str)(Color, const std::string&) = &Logger::log;
    void (Logger::*log_fmt_col_str)(Format format, Color, const std::string&) = &Logger::log;
    void (Logger::*log_col_col_str)(Color, Color, const std::string&) = &Logger::log;
    void (Logger::*log_fmt_col_col_str)(Format format, Color, Color, const std::string&) = &Logger::log;

    void (Logger::*beginProgressLoop_num)(size_t) = &Logger::beginProgressLoop;
    void (Logger::*beginProgressLoop_operation_num)(const std::string&, size_t) = &Logger::beginProgressLoop;

    class_<Logger, boost::noncopyable>("Logger", no_init)
        .def("getInstance", &Logger::getInstance, return_value_policy<reference_existing_object>())
        .staticmethod("getInstance")
        .def("setObjectName", &Logger::setObjectName)
        .def("objectName", &Logger::objectName)
        .def("setLogfile", &Logger::setLogfile)
        .def("log", log_str)
        .def("log", log_fmt_str)
        .def("log", log_col_str)
        .def("log", log_fmt_col_str)
        .def("log", log_col_col_str)
        .def("log", log_fmt_col_col_str)
        .def("logN", &Logger::logN)
        .def("logFlushline", &Logger::logFlushline)
        .def("logTimed", &PyLogger::logTimed)
        .def("warn", &Logger::warn)
        .def("indentation", &PyLogger::indentation)
        .def("indent", &Logger::indent)
        .def("unindent", &Logger::unindent)
        .def("beginProgressLoop", beginProgressLoop_num)
        .def("beginProgressLoop", beginProgressLoop_operation_num)
        .def("progressLoop", PyLogger::progressLoop_num)
        .def("progressLoop", PyLogger::progressLoop_str_num)
        .def("advanceProgressLoop", &Logger::advanceProgressLoop)
        .def("endProgressLoop", &Logger::endProgressLoop)
        .def("startNewTimer", &Logger::startNewTimer)
        .def("endTimer", &Logger::endTimer)
        
        .def("formatHTML", &Logger::formatHTML)
    ;

} //BOOST_PYTHON_MODULE_INIT(_logger)
