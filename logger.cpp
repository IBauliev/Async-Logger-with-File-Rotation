#include "logger.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

Logger::Logger(const std::string& configPath) : running(true) {
    loadConfig();
    if (!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }
    rotateLogFile();
    workerThread = std::thread(&Logger::processLogs, this);
}

Logger::~Logger() {
    stop();
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < minLogLevel) return;

    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");

    std::string levelStr;
    switch (level) {
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }

    std::lock_guard<std::mutex> lock(queueMutex);
    logQueue.emplace(level, "[" + ss.str() + "] [" + levelStr + "] " + message);
    cv.notify_one();
}

void Logger::stop() {
    running = false;
    cv.notify_all();
    if (workerThread.joinable()) {
        workerThread.join();
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::processLogs() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this] { return !logQueue.empty() || !running; });

        while (!logQueue.empty()) {
            auto entry = logQueue.front();
            logQueue.pop();
            lock.unlock();

            if (logFile.tellp() > maxFileSize) {
                rotateLogFile();
            }

            logFile << entry.second << std::endl;

            lock.lock();
        }
    }
}

void Logger::rotateLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y%m%d_%H%M%S");
    currentLogFileName = logDir + "/log_" + ss.str() + ".txt";
    logFile.open(currentLogFileName, std::ios::app);
}

void Logger::loadConfig() {

    std::ifstream configFile("logger_config.json");
    if (!configFile) return;

    std::string line, content;
    while (std::getline(configFile, line)) {
        content += line;
    }

    size_t pos;
    if ((pos = content.find("\"logDir\"")) != std::string::npos) {
        size_t start = content.find(':', pos) + 1;
        size_t end = content.find(',', start);
        logDir = content.substr(start + 1, end - start - 2);
    }

}