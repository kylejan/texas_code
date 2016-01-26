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

    void run();
    void dispatch_pb_message(Heartbeat*);
    void dispatch_pb_message(ConnectRequest*);
    void dispatch_pb_message(ActionRequest*);
    void dispatch_pb_message(ShowDownRequest*);

private:
    std::int64_t request_id_;
};

} // namespace server
} // namespace messenger
