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

	// Установка параметров stdin в неблокирующий режим
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	while ( true ) {
		 try {
			// Ожидание запроса на аутентификацию
			bool has_input = wait_for_input( STDIN_FILENO, 30000 );
			if ( !has_input ) {
				std::clog << "чтение завершено по таймауту" << std::endl;
				continue;
			}
		} catch ( const std::exception& ex ) {
			std::cerr << ex.what() << std::endl;
			continue;
		} catch ( ... ) {
			std::cerr << "unknown error" << std::endl;
			continue;
		}

		line = read_authentication_request();
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

bool					AuthHTTP::wait_for_input( int fd, int timeout_ms ) {
	fd_set	readfds;

	FD_ZERO( &readfds );
	FD_SET(fd, &readfds);

	timeval	timeout;
	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = ( timeout_ms % 1000 ) * 1000;

	int result = select( fd + 1, &readfds, NULL, NULL, &timeout );
	if ( result < 0 ) {
		throw std::runtime_error( "select error" );
	}

	return result > 0;
}

std::string				AuthHTTP::read_authentication_request( void ) {
	uint16_t	length;

	std::cin.read( reinterpret_cast<char*>( &length ), sizeof( length ) );
	length = ntohs( length );
	if ( !std::cin.good() ) {
		throw std::runtime_error( "error reading length from stdin" );
	}
	if ( length < sizeof( length ) ) {
		throw std::runtime_error( "invalid length of authentication request" );
	}
	std::string input( length, '\0' );
	std::cin.read( &input[0], length );
	if ( !std::cin.good() ) {
		throw std::runtime_error("error reading authentication request from stdin");
	}

	return input;
}

std::string 			AuthHTTP::getConfigFilePath( void ) const {
	return configReader.getConfigFilePath();
}

HttpCommandExecutor&	AuthHTTP::getCommandExecutor( void ) {
	return commandExecutor;
}

