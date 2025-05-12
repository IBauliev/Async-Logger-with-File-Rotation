#include "logger.h"
#include <thread>

int main() {
    Logger logger("logger_config.json");

    logger.log(LogLevel::INFO, "Приложение запущено");
    logger.log(LogLevel::WARNING, "Нехватка памяти");
    logger.log(LogLevel::ERROR, "Ошибка подключения к БД");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    logger.stop();
    return 0;
}