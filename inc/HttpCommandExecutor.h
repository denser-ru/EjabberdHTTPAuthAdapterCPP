#pragma once

#include <string>

class HttpCommandExecutor {
public:
    explicit HttpCommandExecutor(const std::string& url);
    bool executeCommand(const std::string& cmd, const std::string& pkt);

private:
    std::string url_;
};
