#pragma once

#include <string>
#include <unordered_map>
#include "FileWrapper.h"

#define DEF_CONFIG_FILE	"/home/ejabberd/auth_http.conf"
// #define DEF_AUTH_LOG	"/home/ejabberd/auth_http.log"
// #define DEF_ERROR_LOG	"/home/ejabberd/auth_http.err"
#define DEF_AUTH_LOG	"./auth_http.log"
#define DEF_ERROR_LOG	"./auth_http.err"
#define DEF_AUTH_URL	"http://localhost:8080"

using ConfigMap = std::unordered_map<std::string, std::string>;

class Config
{
	private:
		ConfigMap&					configMap;
		const std::string			outputFilePath_;
		const std::string			errorLogFilePath_;
		FileWrapper<std::ofstream>	outputFileWrapper_;
		FileWrapper<std::ofstream>	errorLogFileWrapper_;
		std::ofstream&				outputFile;
		std::ofstream&				errorLogFile;

	public:
		explicit Config( ConfigMap& configMap );
		~Config( void );

		std::string				getErrorLogPath( void ) const;
		std::string				getAuthLogPath( void ) const;
		std::ofstream&			getOutputFile( void );
		std::ofstream&			getErrorLogFile( void );

		Config*	operator = ( Config* newConfig );
};

class ConfigReader {
	public:

		ConfigReader( void );
		explicit ConfigReader( const std::string& configFile );
		~ConfigReader( void );

		void	   			readConfig( void );
		std::string			getConfigFilePath( void ) const;
		const Config*	 	getConfig( void ) const;
		const ConfigMap&	getConfigMap( void ) const;
		std::string			getErrorLogPath( void ) const;
		std::string			getAuthLogPath( void ) const;
		std::ofstream&		getOutputFile( void );
		std::ofstream&		getErrorLogFile( void );

    	void				setConfig( void );

	private:
		std::string			configFile_;
		ConfigMap			configMap_;
		Config*				config_;
};
