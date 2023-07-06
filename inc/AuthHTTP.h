#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ConfigReader.h"
#include "HttpCommandExecutor.h"

class AuthHTTP {
	public:
		std::string				getConfigFilePath( void ) const;
		HttpCommandExecutor&	getCommandExecutor( void );

		AuthHTTP( void );
		explicit AuthHTTP( const std::string& configFilePath) ;

		void						run( void );
		const Config*				getConfig( void ) const;

	private:
		ConfigReader				configReader;
		HttpCommandExecutor			commandExecutor;

		std::vector<std::string>	splitString( const std::string& str, char delimiter) ;
};
