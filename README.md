# **Async Logger with File Rotation**  

**Асинхронный логгер с ротацией файлов и фильтрацией сообщений**  

Логгер записывает сообщения в файлы с поддержкой уровней логирования (`INFO`, `WARNING`, `ERROR`), автоматически создает новые файлы при достижении максимального размера и работает в асинхронном режиме.  

## **📌 Особенности**  
✔ **Многопоточность** – запись логов в отдельном потоке без блокировки основного.  
✔ **Ротация логов** – автоматическое создание новых файлов при превышении лимита размера.  
✔ **Фильтрация сообщений** – вывод только нужных уровней (например, только `ERROR`).  
✔ **Конфигурация через JSON** – настройка пути, размера файла и уровня логирования.  
✔ **STL без сторонних библиотек** – только стандартные средства C++17 (`<filesystem>`, `<thread>`, `<mutex>`).  

---

## **🚀 Сборка и запуск**  

### **Требования**  
- Компилятор с поддержкой C++17 (`g++`, `clang`, MSVC).  
- CMake (опционально, для удобной сборки).  

### **Сборка вручную**  
```sh
g++ -std=c++17 -pthread logger.cpp main.cpp -o logger
./logger
```

### **Сборка через CMake**  
1. Создайте `CMakeLists.txt`:  
```cmake
cmake_minimum_required(VERSION 3.10)
project(AsyncLogger)

set(CMAKE_CXX_STANDARD 17)

add_executable(logger logger.cpp main.cpp)
target_link_libraries(logger pthread)
```

2. Соберите и запустите:  
```sh
mkdir build && cd build
cmake ..
make
./logger
```

---

## **⚙ Конфигурация**  
Создайте файл `logger_config.json` в той же директории:  
```json
{
    "logDir": "logs",
    "maxFileSize": 1048576,
    "minLogLevel": "INFO"
}
```
- `logDir` – папка для хранения логов (если не существует, создается автоматически).  
- `maxFileSize` – максимальный размер файла в байтах (по умолчанию 1 МБ).  
- `minLogLevel` – минимальный уровень логирования (`INFO`, `WARNING`, `ERROR`).  

---

## **📝 Пример использования**  
```cpp
#include "logger.h"
#include <thread>

int main() {
    Logger logger("logger_config.json");

    logger.log(LogLevel::INFO, "Приложение запущено");
    logger.log(LogLevel::WARNING, "Нехватка памяти");
    logger.log(LogLevel::ERROR, "Ошибка подключения к БД");

    // Имитация работы приложения
    std::this_thread::sleep_for(std::chrono::seconds(1));

    logger.stop(); // Корректное завершение
    return 0;
}
```

---

## **📂 Структура проекта**  
```
├── CMakeLists.txt          # Конфигурация CMake
├── logger_config.json      # Настройки логгера
├── logger.h                # Заголовочный файл
├── logger.cpp              # Реализация логгера
└── main.cpp                # Пример использования
```

---

## **📌 Возможные улучшения**  
🔹 **Добавить тесты** (Google Test).  
🔹 **Реализовать нормальный JSON-парсинг** (например, через `nlohmann/json`).  
🔹 **Добавить сетевую отправку логов** (например, через UDP/TCP).  
🔹 **Поддержка Unicode** (для кириллицы и других языков).  
