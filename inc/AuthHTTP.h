#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ConfigReader.h"
#include "HttpCommandExecutor.h"
#include "FileWrapper.h"

class AuthHTTP {
public:
    std::string				getConfigFilePath( void ) const;
    std::string				getUrl( void );
    std::string				getErrorLogPath( void ) const;
    std::string				getAuthLogPath( void ) const;
    HttpCommandExecutor&	getCommandExecutor( void );
    std::ofstream&			getOutputFile( void );
    std::ofstream&			getErrorLogFile( void );

    AuthHTTP( void );
    explicit AuthHTTP( const std::string& configFilePath) ;

    void	setConfigFilePath( const std::string& configFilePath );
    void	run( void );

private:
    ConfigReader				configReader;
    ConfigMap					config;
    std::string					url;
    std::string					errorLogPath;
    std::string					authLogPath;
    HttpCommandExecutor			commandExecutor;
    FileWrapper<std::ofstream>	outputFileWrapper;
    FileWrapper<std::ofstream>	errorLogFileWrapper;
    std::ofstream&				outputFile;
    std::ofstream&				errorLogFile;

    std::vector<std::string>	splitString( const std::string& str, char delimiter) ;
};
