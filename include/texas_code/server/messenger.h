#pragma once

#include "texas_code/protocol/raw_message.h"
#include "texas_code/protocol/message.pb.h"
#include "texas_code/protocol/message_type.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>

#include <boost/asio/io_service.hpp>

namespace texas_code {
namespace server {

using namespace protocol;

class Messenger {
public:
    Messenger() = delete;
    Messenger(const Messenger&) = delete;
    Messenger(Messenger&&) = delete;
    Messenger& operator = (const Messenger&) = delete;
    Messenger& operator = (Messenger&&) = delete;

    Messenger(const std::string& rpc_endpoint, const std::string& pub_endpoint);

    inline boost::asio::io_service& get_service();
    inline void socket_rpc_recv();
    inline void socket_rpc_reply(RawMessage* raw_message);
    inline void socket_pub_send(RawMessage* raw_message);

    virtual void init();
    virtual void run();
    virtual void dispatch_message(std::unique_ptr<Heartbeat>) = 0;
    virtual void dispatch_message(std::unique_ptr<ConnectRequest>) = 0;
    virtual void dispatch_message(std::unique_ptr<ActionRequest>) = 0;
    virtual void dispatch_message(std::unique_ptr<ShowDownRequest>) = 0;

    virtual ~Messenger() {}

private:
    template <typename type>
    void dispatch(std::unique_ptr<RawMessage> raw_message) {
        std::unique_ptr<type> message(new type());
        message->ParseFromString(raw_message->msg_body);
        dispatch_message(std::move(message));
    }

    void handle_recv_message(std::unique_ptr<RawMessage> raw_message);

    zmq::context_t context_;
    zmq::socket_t rpc_socket_;
    zmq::socket_t pub_socket_;

    std::string rpc_endpoint_;
    std::string pub_endpoint_;

    std::mutex mutex_;
    std::atomic<bool> is_wait_{false};
    std::condition_variable cv_;

    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_;
};

} // namespace server
} // namespace texas_code
