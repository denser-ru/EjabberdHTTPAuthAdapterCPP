#pragma once

#include <string>

class HttpCommandExecutor {
public:
    explicit HttpCommandExecutor( void ) = default;
    explicit HttpCommandExecutor( const std::string& url );

    bool    executeCommand( const std::string& cmd, const std::string& pkt );
    void	setUrl( const std::string& url );

private:
    std::string url_;
};
