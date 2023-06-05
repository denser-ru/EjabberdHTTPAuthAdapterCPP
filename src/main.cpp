#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include "ConfigReader.h"
#include "HttpCommandExecutor.h"

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    try {
        ConfigReader configReader("auth.conf");
        ConfigMap config = configReader.readConfig();

        std::string url = config["url"];
        std::string errorLogPath = config["error_log"];

        HttpCommandExecutor commandExecutor(url);

        while (true) {
            // Чтение длины данных
            short dataLength;
            std::cin.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));

            // Чтение строки данных из стандартного ввода
            std::string input;
            std::getline(std::cin, input);

            // Обработка команды
            std::vector<std::string> lines = splitString(input, '\n');
            for (const std::string& line : lines) {
                // Отправка заголовка HTTP для каждой строки
                std::cout << "Content-Length: " << line.size() << std::endl;

                // Обработка команды
                std::vector<std::string> tokens = splitString(line, ':');
                if (tokens.empty()) {
                    std::cerr << "Неверный формат команды" << std::endl;
                    continue;
                }

                const std::string& cmd = tokens[0];
                std::string pkt;
                if (tokens.size() > 1) {
                    pkt = line.substr(line.find(':') + 1);
                }

                // Отправка данных команды
                std::cout << "cmd: " << cmd << std::endl;
                std::cout << "pkt: " << pkt << std::endl;

                // Проверка на выход из цикла
                if (cmd == "exit") {
                    // std::cerr << "успешное завершение"<< std::endl;
                    return EXIT_SUCCESS;
                }

                // Выполнение команды по HTTP
                bool response = commandExecutor.executeCommand(cmd, pkt); // Обновленный вызов метода

                // Обработка ответа
                // ...

                // Запись результата в стандартный вывод
                std::cout << (response ? "\x00\x02\x00\x01" : "\x00\x02\x00\x00") << std::endl;
            }
        }
    } catch (const std::exception& e) {
        // Запись ошибки в журнал ошибок
        // ...
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
