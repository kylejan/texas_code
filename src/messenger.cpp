#include "texas_code/server/messenger.h"

namespace texas_code {

namespace server {

Messenger::Messenger(const std::string& rpc_endpoint, const std::string& pub_endpoint)
    : rpc_endpoint_{rpc_endpoint}
    , pub_endpoint_{pub_endpoint}
    , context_{1}
    , rpc_socket_{context_, ZMQ_REP}
    , pub_socket_{context_, ZMQ_PUB}
    , work_(io_service_) {

    rpc_socket_.bind(rpc_endpoint_);
    pub_socket_.bind(pub_endpoint_);
}

void Messenger::init() {
    get_service().post([this] {
        while (true) {
            socket_rpc_recv();
        }
    });
}

void Messenger::run() {
    get_service().run();
}

boost::asio::io_service& Messenger::get_service() {
    return io_service_;
}

void Messenger::socket_rpc_recv() {
    zmq::message_t request;
    rpc_socket_.recv(&request);
    std::unique_ptr<RawMessage> raw_message(new RawMessage(&request));
    handle_recv_message(std::move(raw_message));

    std::unique_lock<std::mutex> lock(mutex_);
    is_wait_ = true;
    cv_.wait(lock, [this] { return is_wait_.load() == false; });
}

void Messenger::socket_rpc_reply(RawMessage* raw_message) {
    get_service().post([this, raw_message]{
        zmq::message_t message = raw_message->pack_zmq_msg();
        rpc_socket_.send(message);
        delete raw_message;
        is_wait_ = false;
        cv_.notify_one();
    });
}

void Messenger::socket_pub_send(RawMessage* raw_message) {
    get_service().post([this, raw_message]{
        zmq::message_t message = raw_message->pack_zmq_msg();
        pub_socket_.send(message);
        delete raw_message;
    });
}

void Messenger::handle_recv_message(std::unique_ptr<RawMessage> raw_message) {
    if (raw_message == nullptr) return;

    switch (static_cast<MessageType>(raw_message->msg_type)) {
        case MessageType::UNKNOWN_REQUEST:
            return;
        case MessageType::HEARTBEAT:
            dispatch<Heartbeat>(std::move(raw_message));
            break;
        case MessageType::ACTION_REQUEST:
            dispatch<ActionRequest>(std::move(raw_message));
            break;
        case MessageType::CONNECT_REQUEST:
            dispatch<ConnectRequest>(std::move(raw_message));
            break;
        case MessageType::SHOW_DOWN_REQUEST:
            dispatch<ShowDownRequest>(std::move(raw_message));
            break;
        default:
            break;
    }
}

} // namespace server
} // namespace texas_code
