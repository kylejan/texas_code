#pragma once

#include "texas_code/protocol/raw_message.h"
#include "texas_code/protocol/message.pb.h"
#include "texas_code/protocol/message_type.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>

#include <iostream>

namespace texas_code {
namespace server {

using namespace protocol;

class Messenger {
public:
    Messenger() = delete;
    Messenger(const std::string& rpc_endpoint, const std::string& pub_endpoint);

    inline void run();
    inline void socket_rpc_recv();
    inline void socket_rpc_reply(RawMessage* raw_message);
    inline void socket_pub_send(RawMessage* raw_message);

    virtual void dispatch_pb_message(Heartbeat*) = 0;
    virtual void dispatch_pb_message(ConnectRequest*) = 0;
    virtual void dispatch_pb_message(ActionRequest*) = 0;
    virtual void dispatch_pb_message(ShowDownRequest*) = 0;

    virtual ~Messenger() {}

private:
    template <typename type>
    void dispatch(RawMessage* raw_message) {
        auto message = new type();
        message->ParseFromString(raw_message->msg_body);
        dispatch_pb_message(message);
    }

    void handle_recv_message(RawMessage* raw_message);

    zmq::context_t context_;
    zmq::socket_t rpc_socket_;
    zmq::socket_t pub_socket_;

    std::thread* rpc_thread_;

    std::string rpc_endpoint_;
    std::string pub_endpoint_;

    std::mutex mutex_;
    std::atomic<bool> is_wait_{false};
    std::condition_variable cv_;
};

} // namespace server
} // namespace texas_code
