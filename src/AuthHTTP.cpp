#include "AuthHTTP.h"
#include "StreamRedirector.h"

AuthHTTP::AuthHTTP( void )
	:
		configReader()
{}

AuthHTTP::AuthHTTP( const std::string& configFilePath )
	:
		configReader( configFilePath )
{}

void AuthHTTP::run( void ) {
	std::string			line;
	std::string			cmd;
	std::string			pkt;
	bool				response;
	const ConfigMap&	configMap = configReader.getConfigMap();

	try {
		configReader.readConfig();
	}
	catch (const std::exception& ex) {
		// Обработка исключения внутри функции readConfig()
		std::cerr << "Exception occurred in readConfig(): " << ex.what() << std::endl;
		throw;  // Передача исключения дальше
	}

	StreamRedirector coutRedirector( std::clog, configReader.getOutputFile().rdbuf() );
	StreamRedirector cerrRedirector( std::cerr, configReader.getErrorLogFile().rdbuf() );

	commandExecutor.setUrl( configMap.at( "url" ) );

	while ( true ) {
		short dataLength;
		std::cin.read( reinterpret_cast<char*>( &dataLength ), sizeof( dataLength ) );

		if ( !std::cin ) {
			break;
		}

		std::getline( std::cin, line );

		std::vector<std::string> lines = splitString( line, '\n' );
		for ( const std::string& line : lines ) {
			std::clog << "Content-Length: " << line.size() << std::endl;

			std::vector<std::string> tokens = splitString( line, ':' );
			if ( tokens.empty() ) {
				std::cerr << "Неверный формат команды" << std::endl;
				continue;
			}

			cmd = tokens[0];
			pkt = line.substr(line.find(':') + 1);

			if ( cmd == "exit" ) {
				std::clog << "успешное завершение" << std::endl;

				// Выход из цикла и завершение программы
				return;
			}

			std::clog << "url: " << configMap.at("url") << std::endl;

			response = commandExecutor.executeCommand( cmd, pkt );

			coutRedirector.disable(); // Отключение перенаправления на файл

			if ( response )  {
				std::cout.write("\x00\x02\x00\x01", 4);
			} else {
				std::cout.write("\x00\x02\x00\x00", 4);
			}
			std::cout << std::endl;

			coutRedirector.enable(); // Включение перенаправления обратно на файл
		}
	}
}

std::vector<std::string>	AuthHTTP::splitString( const std::string& str, char delimiter ) {
	std::vector<std::string>	tokens;
	std::string					token;
	std::istringstream			iss( str );

	while ( std::getline( iss, token, delimiter ) ) {
		tokens.push_back( token );
	}

	return tokens;
}

const Config*			AuthHTTP::getConfig( void ) const {
	return ( const Config* )configReader.getConfig();
}

std::string 			AuthHTTP::getConfigFilePath( void ) const {
	return configReader.getConfigFilePath();
}

HttpCommandExecutor&	AuthHTTP::getCommandExecutor( void ) {
	return commandExecutor;
}

