#pragma once

#include <string>
#include <unordered_map>

using ConfigMap = std::unordered_map<std::string, std::string>;

class ConfigReader {
public:
    explicit ConfigReader( const std::string& configFile );

    ConfigMap       readConfig( void );
    std::string     getConfigFilePath( void ) const;

private:
    std::string configFile_;
};
