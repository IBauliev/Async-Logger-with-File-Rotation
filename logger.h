#pragma once
#include <string>
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
    Logger(const std::string& configPath);
    ~Logger();

    void log(LogLevel level, const std::string& message);
    void stop();

private:
    void processLogs();
    void loadConfig();
    void rotateLogFile();

    std::ofstream logFile;
    std::string currentLogFileName;
    std::queue<std::pair<LogLevel, std::string>> logQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::thread workerThread;
    std::atomic<bool> running;

    // Конфигурация
    std::string logDir = "logs";
    size_t maxFileSize = 1024 * 1024; // 1 MB
    LogLevel minLogLevel = LogLevel::INFO;
};