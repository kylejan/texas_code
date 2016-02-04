#include "texas_code/server/server.h"

#include <spdlog/spdlog.h>

int main() {
    size_t q_size = 1048576;
    spdlog::set_async_mode(q_size);
    spdlog::stdout_logger_mt("console");

    auto* server = new texas_code::server::Server("tcp://*:5555", "tcp://*:5556");
    server->init();
    server->run();
    return 0;
}
