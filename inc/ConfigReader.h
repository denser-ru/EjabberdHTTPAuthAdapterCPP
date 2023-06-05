#pragma once

#include <string>
#include <map>

using ConfigMap = std::map<std::string, std::string>;

class ConfigReader {
public:
    explicit ConfigReader(const std::string& configFile);
    ConfigMap readConfig();

private:
    std::string configFile_;
};
