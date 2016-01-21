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
    Messenger(const std::string& rpc_endpoint, const std::string& pub_endpoint)
        : rpc_endpoint_{rpc_endpoint}
        , pub_endpoint_{pub_endpoint}
        , context_{1}
        , rpc_socket_{context_, ZMQ_REP}
        , pub_socket_{context_, ZMQ_PUB} {

        rpc_socket_.bind(rpc_endpoint_);
        pub_socket_.bind(pub_endpoint_);
    }

    void run() {
        socket_rpc_recv();
    }

    void socket_rpc_recv() {
        while (true) {
            zmq::message_t request;
            rpc_socket_.recv(&request);
            RawMessage raw_message(&request);

            std::unique_lock<std::mutex> lock(mutex_);
            is_wait_ = true;
            cv_.wait(lock, [this] { return is_wait_.load() == false; });
        }
    }

    void socket_rpc_reply(RawMessage* raw_message) {
        zmq::message_t message = raw_message->pack_zmq_msg();
        rpc_socket_.send(message);
        is_wait_ = false;
        cv_.notify_one();
    }

    void socket_pub_send(RawMessage* raw_message) {
        zmq::message_t message = raw_message->pack_zmq_msg();
        pub_socket_.send(message);
    }

private:
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
