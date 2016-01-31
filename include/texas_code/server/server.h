#pragma once

#include "texas_code/server/messenger.h"

namespace texas_code {

namespace server {

class Server : public Messenger {
public:
    Server() = delete;
    Server(const Server&) = delete;
    Server& operator = (const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator = (Server&&) = delete;

    Server(const std::string&, const std::string&);

    void init();
    void run();
    void dispatch_message(std::unique_ptr<Heartbeat>);
    void dispatch_message(std::unique_ptr<ConnectRequest>);
    void dispatch_message(std::unique_ptr<ActionRequest>);
    void dispatch_message(std::unique_ptr<ShowDownRequest>);

private:
    std::int64_t request_id_;
};

} // namespace server
} // namespace messenger
