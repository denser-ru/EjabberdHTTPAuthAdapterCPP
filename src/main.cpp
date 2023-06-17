#include <cstdlib>
#include <iostream>
#include "AuthHTTP.h"

int main( void ) {
    try {
        AuthHTTP    app;
        app.setConfigFilePath( "auth.conf" );
        app.run();

        return EXIT_SUCCESS;
    } catch ( int exitCode ) {
        // Запись ошибки в журнал ошибок
        // ...

        return exitCode;
    }
}
