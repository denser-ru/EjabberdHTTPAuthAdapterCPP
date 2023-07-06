#include <cstdlib>
#include <cstring>
#include <iostream>
#include "AuthHTTP.h"

int main( void ) {
    try {
        std::string auth_conf;
        char* confEnv = std::getenv( "AUTH_HTTP_CONFIG_FILE" );

        if ( confEnv == nullptr || std::strlen( confEnv ) == 0 ) {
            auth_conf = DEF_CONFIG_FILE;
        } else {
            auth_conf = confEnv;
        }

        AuthHTTP app( auth_conf );
        try {
            app.run();
        } catch ( const std::runtime_error& ex ) {
            // Обработка исключения типа std::runtime_error
            std::cerr << "Exception occurred in app.run(): " << ex.what() << std::endl;
            throw; // Передача исключения дальше
        }

        return EXIT_SUCCESS;
    } catch ( const std::exception& ex ) {
        // Обработка исключений типа std::exception
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    } catch ( int exitCode ) {
        // Обработка исключений типа int
        std::cerr << "Error occurred with exit code: " << exitCode << std::endl;
        return exitCode;
    }
}
