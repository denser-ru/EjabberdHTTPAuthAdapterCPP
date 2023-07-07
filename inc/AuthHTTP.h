#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <arpa/inet.h>
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
		bool						wait_for_input(int fd, int timeout_ms);
		std::string					read_authentication_request( void );

	private:
		ConfigReader				configReader;
		HttpCommandExecutor			commandExecutor;

		std::vector<std::string>	splitString( const std::string& str, char delimiter) ;
};
