#pragma once
#include <iostream>
#include <string>

// ANSI color codes for terminal output
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"
#define DIM     "\033[2m"

enum class LogLevel { INFO, SUCCESS, WARNING, ERROR, DEBUG };

// ─────────────────────────────────────────────────────────────────────────────
// Logger  –  Singleton pattern
// Demonstrates: Single Responsibility Principle (logging only)
//              Open/Closed Principle (extend levels without modifying core)
// ─────────────────────────────────────────────────────────────────────────────
class Logger {
private:
    Logger() {}

    std::string levelTag(LogLevel level) const {
        switch (level) {
            case LogLevel::INFO:    return std::string(CYAN)    + "[INFO]   " + RESET;
            case LogLevel::SUCCESS: return std::string(GREEN)   + "[  OK  ] " + RESET;
            case LogLevel::WARNING: return std::string(YELLOW)  + "[ WARN ] " + RESET;
            case LogLevel::ERROR:   return std::string(RED)     + "[ERROR]  " + RESET;
            case LogLevel::DEBUG:   return std::string(MAGENTA) + "[DEBUG]  " + RESET;
            default:                return "[LOG]    ";
        }
    }

public:
    // Deleted copy/assign to enforce singleton
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string &component, const std::string &msg) const {
        std::cout << levelTag(level)
                  << BOLD << BLUE << "[" << component << "]" << RESET
                  << "  " << msg << "\n";
    }

    // Convenience wrappers
    void info   (const std::string &c, const std::string &m) const { log(LogLevel::INFO,    c, m); }
    void success(const std::string &c, const std::string &m) const { log(LogLevel::SUCCESS, c, m); }
    void warn   (const std::string &c, const std::string &m) const { log(LogLevel::WARNING, c, m); }
    void error  (const std::string &c, const std::string &m) const { log(LogLevel::ERROR,   c, m); }
    void debug  (const std::string &c, const std::string &m) const { log(LogLevel::DEBUG,   c, m); }

    void separator(const std::string &title = "") const {
        std::cout << DIM << "\n──────────────────────────────────────────────────\n" << RESET;
        if (!title.empty())
            std::cout << BOLD << "  " << title << "\n" << RESET;
        std::cout << DIM << "──────────────────────────────────────────────────\n\n" << RESET;
    }
};

// Global convenience macro
#define LOG Logger::getInstance()
