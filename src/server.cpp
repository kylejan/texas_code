#include "texas_code/server/server.h"

namespace texas_code {

namespace server {

Server::Server(const std::string& rpc_endpoint, const std::string& pub_endpoint)
    : Messenger(rpc_endpoint, pub_endpoint)
    , request_id_(0) {

}

void Server::init() {
    Messenger::init();
}

void Server::run() {
    Messenger::run();
}

void Server::dispatch_pb_message(Heartbeat* request) {

}

void Server::dispatch_pb_message(ConnectRequest* request) {

}

void Server::dispatch_pb_message(ActionRequest* request) {

}

void Server::dispatch_pb_message(ShowDownRequest* request) {

}

} // namespace server
} // namespace texas_code
