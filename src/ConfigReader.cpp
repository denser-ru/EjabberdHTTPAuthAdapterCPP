#include "ConfigReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>

ConfigReader::ConfigReader(const std::string& configFile) : configFile_(configFile) {}

ConfigMap ConfigReader::readConfig() {
    std::ifstream configFileStream(configFile_);
    if (!configFileStream.is_open()) {
        throw std::runtime_error("Failed to open config file: " + configFile_);
    }

    ConfigMap configMap;
    std::string line;
    while (std::getline(configFileStream, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            configMap[key] = value;
        }
    }

    configFileStream.close();
    return configMap;
}

std::string ConfigReader::getConfigFilePath() const {
    return configFile_;
}
