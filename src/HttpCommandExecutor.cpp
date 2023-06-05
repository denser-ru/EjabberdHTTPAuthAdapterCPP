#include "HttpCommandExecutor.h"
#include <iostream>
#include <curl/curl.h>

HttpCommandExecutor::HttpCommandExecutor(const std::string& url) : url_(url) {}

// Функция обратного вызова для обработки полученных данных
size_t writeCallback(char* data, size_t size, size_t nmemb, std::string* responseData) {
    if (responseData == nullptr) {
        return 0;
    }

    size_t totalSize = size * nmemb;
    responseData->append(data, totalSize);

    return totalSize;
}

bool HttpCommandExecutor::executeCommand(const std::string& cmd, const std::string& pkt) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return false;
    }

    std::string requestData = cmd + ":" + pkt;

    // Установка URL
    curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());

    // Установка метода запроса POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Установка тела запроса
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());

    // Установка функции обратного вызова для обработки полученных данных
    std::string responseData;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    // Выполнение запроса
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to execute command: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    std::cout << "Response: " << responseData << std::endl;

    return true;
}
