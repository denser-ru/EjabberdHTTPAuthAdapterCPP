#include "AuthHTTP.h"
#include "StreamRedirector.h"

AuthHTTP::AuthHTTP( void )
    : configReader("auth.conf"), url(), errorLogPath(), authLogPath(),
        commandExecutor(), outputFileWrapper("auth.log"), errorLogFileWrapper("error.log"),
        outputFile(outputFileWrapper.getFile()), errorLogFile(errorLogFileWrapper.getFile()) {}

AuthHTTP::AuthHTTP( const std::string& configFilePath )
    : configReader(configFilePath), url( getUrl() ), errorLogPath(), authLogPath(),
        commandExecutor(), outputFileWrapper("auth.log"), errorLogFileWrapper("error.log"),
        outputFile(outputFileWrapper.getFile()), errorLogFile(errorLogFileWrapper.getFile()) {}

void AuthHTTP::setConfigFilePath( const std::string& configFilePath ) {
    configReader = ConfigReader( configFilePath );
}

void AuthHTTP::run() {
    // Реализация функции run()
    StreamRedirector coutRedirector( std::cout, outputFile.rdbuf() );
    StreamRedirector cerrRedirector( std::cerr, errorLogFile.rdbuf() );
    // std::cerr << "Перенаправили ошибки в файл..." << std::endl;

    std::string     line;
    std::string     cmd;
    std::string     pkt;
    bool            response;

    config = configReader.readConfig();
    commandExecutor.setUrl( config["url"] );

    while ( true ) {
        short dataLength;
        std::cin.read( reinterpret_cast<char*>( &dataLength ), sizeof( dataLength ) );

        if ( !std::cin ) {
            break;
        }

        std::getline( std::cin, line );

        std::vector<std::string> lines = splitString( line, '\n' );
        for ( const std::string& line : lines ) {
            std::cout << "Content-Length: " << line.size() << std::endl;

            std::vector<std::string> tokens = splitString( line, ':' );
            if ( tokens.empty() ) {
                std::cerr << "Неверный формат команды" << std::endl;
                continue;
            }

            cmd = tokens[0];
            pkt = line.substr(line.find(':') + 1);

            if ( cmd == "exit" ) {
                std::cout << "успешное завершение" << std::endl;

                // Выход из цикла и завершение программы
                return;
            }

            std::cout << "url: " << config["url"] << std::endl;

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

std::vector<std::string>    AuthHTTP::splitString( const std::string& str, char delimiter ) {
    std::vector<std::string>    tokens;
    std::string                 token;
    std::istringstream          iss( str );
    while ( std::getline( iss, token, delimiter ) ) {
        tokens.push_back( token );
    }
    return tokens;
}

std::string AuthHTTP::getConfigFilePath( void ) const {
    return configReader.getConfigFilePath();
}

std::string AuthHTTP::getUrl( void ) {
    return config["url"];
}

std::string AuthHTTP::getErrorLogPath( void ) const {
    return errorLogPath;
}

std::string AuthHTTP::getAuthLogPath( void ) const {
    return authLogPath;
}

HttpCommandExecutor& AuthHTTP::getCommandExecutor( void ) {
    return commandExecutor;
}

std::ofstream& AuthHTTP::getOutputFile( void ) {
    return outputFile;
}

std::ofstream& AuthHTTP::getErrorLogFile( void ) {
    return errorLogFile;
}
