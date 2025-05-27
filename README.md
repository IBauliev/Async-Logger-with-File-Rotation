**Async Logger with File Rotation**  

This is an asynchronous logger with file rotation and message filtering. The logger writes messages to files with support for different log levels (INFO, WARNING, ERROR), automatically creates new files when the maximum size is reached, and operates asynchronously.  

**Key Features**  
- **Multithreading** – Logs are written in a separate thread without blocking the main program.  
- **Log Rotation** – Automatically creates new log files when the size limit is exceeded.  
- **Message Filtering** – Only logs messages of the specified level (e.g., only ERROR).  
- **JSON Configuration** – Supports configuration via a JSON file for log directory, max file size, and log level.  
- **STL Only** – Uses only C++17 standard libraries (`<filesystem>`, `<thread>`, `<mutex>`) with no external dependencies.  

**Build and Run**  

**Requirements**  
- A C++17-compatible compiler (g++, clang, MSVC).  
- CMake (optional, for easier build setup).  

**Manual Build**  
```sh
g++ -std=c++17 -pthread logger.cpp main.cpp -o logger  
./logger  
```  

**CMake Build**  
1. Create a `CMakeLists.txt` file:  
```cmake
cmake_minimum_required(VERSION 3.10)  
project(AsyncLogger)  

set(CMAKE_CXX_STANDARD 17)  

add_executable(logger logger.cpp main.cpp)  
target_link_libraries(logger pthread)  
```  

2. Build and run:  
```sh
mkdir build && cd build  
cmake ..  
make  
./logger  
```  

**Configuration**  
Create a `logger_config.json` file in the same directory:  
```json
{
    "logDir": "logs",  
    "maxFileSize": 1048576,  
    "minLogLevel": "INFO"  
}  
```  
- `logDir` – Directory for storing log files (created automatically if it doesn't exist).  
- `maxFileSize` – Maximum log file size in bytes (default: 1 MB).  
- `minLogLevel` – Minimum log level (INFO, WARNING, ERROR).  

**Example Usage**  
```cpp
#include "logger.h"  
#include <thread>  

int main() {  
    Logger logger("logger_config.json");  

    logger.log(LogLevel::INFO, "Application started");  
    logger.log(LogLevel::WARNING, "Low memory warning");  
    logger.log(LogLevel::ERROR, "Database connection error");  

    std::this_thread::sleep_for(std::chrono::seconds(1));  

    logger.stop(); // Proper shutdown  
    return 0;  
}  
```  

**Project Structure**  
```
CMakeLists.txt          # CMake configuration  
logger_config.json      # Logger settings  
logger.h                # Header file  
logger.cpp              # Logger implementation  
main.cpp                # Example usage  
```  

**Possible Improvements**  
- Add unit tests (e.g., Google Test).  
- Implement proper JSON parsing (e.g., using nlohmann/json).  
- Add network logging support (UDP/TCP).  
- Improve Unicode support (for Cyrillic and other languages).
