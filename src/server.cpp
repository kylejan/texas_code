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

void Server::dispatch_message(std::unique_ptr<Heartbeat> request) {

}

void Server::dispatch_message(std::unique_ptr<ConnectRequest> request) {

}

void Server::dispatch_message(std::unique_ptr<ActionRequest> request) {

}

void Server::dispatch_message(std::unique_ptr<ShowDownRequest> request) {

}

} // namespace server
} // namespace texas_code
