#include "texas_code/server/server.h"

int main() {
    auto* server = new texas_code::server::Server("tcp://*:5555", "tcp://*:5556");
    server->run();
    return 0;
}
