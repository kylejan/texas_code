#pragma once

#include <cstdint>
#include <zmq.hpp>

namespace texas_code {

namespace protocol {

struct RawMessage {
    std::int32_t msg_type;
    std::string msg_body;

    RawMessage() = delete;
    RawMessage(std::int32_t type, const std::string& body)
        : msg_type(type)
        , msg_body(body)
    {}

    RawMessage(zmq::message_t* msg) {
        memcpy(&msg_type, (void*)msg->data(), sizeof(msg_type));
        size_t body_size = msg->size() - sizeof(msg_type);
        char buffer[body_size + 1];
        memcpy(buffer, (char*)msg->data() + sizeof(msg_type), body_size);
        msg_body = std::string(buffer, body_size);
    }

    zmq::message_t pack_zmq_msg() {
        zmq::message_t msg(sizeof(msg_type) + msg_body.size());
        memcpy((void*)msg.data(), &msg_type, sizeof(msg_type));
        memcpy((char*)msg.data() + sizeof(msg_type), msg_body.c_str(), msg_body.size());
        return msg;
    }
};

} // namespace protocol
} // namespace texas_code
