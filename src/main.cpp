#include "texas_code/server/messenger.h"

int main() {
    auto* messenger = new texas_code::server::Messenger("tcp://*:5555", "tcp://*:5556");
    messenger->run();
    return 0;
}
