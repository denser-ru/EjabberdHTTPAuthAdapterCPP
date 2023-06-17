#pragma once

#include <string>
#include <fstream>

template<typename T>
class FileWrapper {
public:
    explicit FileWrapper( const std::string& filePath );
    ~FileWrapper( void );

    T&  getFile( void );

private:
    T file;
};
