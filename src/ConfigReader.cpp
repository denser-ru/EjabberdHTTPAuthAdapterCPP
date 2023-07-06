#include "ConfigReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>

Config::Config( ConfigMap& configMap )
	:
		configMap( configMap ),
		outputFilePath_( configMap.count( "auth_log" ) ? configMap.at( "auth_log" ) : DEF_AUTH_LOG ),
		errorLogFilePath_( configMap.count( "error_log" ) ? configMap.at( "error_log" ) : DEF_ERROR_LOG ),
		outputFileWrapper_( outputFilePath_ ),
		errorLogFileWrapper_( errorLogFilePath_ ),
		outputFile( outputFileWrapper_.getFile() ),
		errorLogFile( errorLogFileWrapper_.getFile() )
{
	if ( !outputFileWrapper_.isFileOpen() ) {
		// Если файл auth_log недоступен
		// Обработка ошибки...
		throw std::runtime_error( "Failed to open auth_log file: " + outputFilePath_ );
	}

	if ( !errorLogFileWrapper_.isFileOpen() ) {
		// Если файл error_log недоступен
		// Обработка ошибки...
		throw std::runtime_error( "Failed to open error_log file: " + errorLogFilePath_ );
	}
}

Config::~Config( void ) {}

ConfigReader::ConfigReader( void )
	:
		configFile_( DEF_CONFIG_FILE ),
		configMap_(),
		config_( NULL )
{}

ConfigReader::ConfigReader( const std::string& configFile )
	:
		configFile_( configFile ),
		configMap_(),
		config_( NULL )
{}

ConfigReader::~ConfigReader( void ) {
	delete config_;
}

void ConfigReader::readConfig( void ) {
	std::ifstream configFileStream( configFile_ );

	if (!configFileStream.is_open()) {
		throw std::runtime_error( "Failed to open config file: " + configFile_ );
	}

	std::string line;

	while ( std::getline( configFileStream, line ) ) {
		if ( line.empty() || line[0] == '#' ) {
			continue;
		}

		std::istringstream iss( line );
		std::string key, value;
		if ( std::getline( iss, key, ':' ) && std::getline( iss, value )) {
			configMap_[key] = value;
		}
	}

	if ( !configMap_.count("auth_log") || configMap_["auth_log"].empty() )
		configMap_["auth_log"] = DEF_AUTH_LOG;
	if ( !configMap_.count("error_log") || configMap_["error_log"].empty() )
		configMap_["error_log"] = DEF_ERROR_LOG;
	if ( !configMap_.count("url") || configMap_["url"].empty() )
		configMap_["url"] = DEF_AUTH_URL;

	configFileStream.close();

	setConfig();
}

std::string ConfigReader::getConfigFilePath( void ) const {
	return configFile_;
}

const Config* ConfigReader::getConfig( void ) const {
	return config_;
}

const ConfigMap& ConfigReader::getConfigMap( void ) const {
	return configMap_;
}

// std::string ConfigReader::getUrl( void ) {
//	 return configMap["url"];
// }

// std::string ConfigReader::getErrorLogPath( void ) const {
//	 return config.getErrorLogPath();
// }

// std::string ConfigReader::getAuthLogPath( void ) const {
//	 return config.getAuthLogPath();
// }

std::ofstream&	Config::getOutputFile( void ) {
	return outputFile;
}

std::ofstream&	Config::getErrorLogFile( void ) {
	return errorLogFile;
}

std::ofstream&	ConfigReader::getOutputFile( void ) {
	return config_->getOutputFile();
}

std::ofstream&	ConfigReader::getErrorLogFile( void ) {
	return config_->getErrorLogFile();
}

void			ConfigReader::setConfig( void )
{
	config_ = new Config( configMap_ );
}

Config*	Config::operator = ( Config* newConfig) {
	return newConfig;
}