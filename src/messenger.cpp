#include "texas_code/server/messenger"

namespace texas_code {

namespace server {

Messenger::Messenger(const std::string& rpc_endpoint, const std::string& pub_endpoint)
    : rpc_endpoint_{rpc_endpoint}
    , pub_endpoint_{pub_endpoint}
    , context_{1}
    , rpc_socket_{context_, ZMQ_REP}
    , pub_socket_{context_, ZMQ_PUB} {

    rpc_socket_.bind(rpc_endpoint_);
    pub_socket_.bind(pub_endpoint_);
}

void Messenger::run() {
    rpc_thread_ = new std::thread(&Messenger::socket_rpc_recv, this);
    rpc_thread_->join();
}

void Messenger::socket_rpc_recv() {
    while (true) {
        zmq::message_t request;
        rpc_socket_.recv(&request);
        RawMessage raw_message(&request);

        std::unique_lock<std::mutex> lock(mutex_);
        is_wait_ = true;
        cv_.wait(lock, [this] { return is_wait_.load() == false; });
    }
}

void Messenger::socket_rpc_reply(RawMessage* raw_message) {
    zmq::message_t message = raw_message->pack_zmq_msg();
    rpc_socket_.send(message);
    is_wait_ = false;
    cv_.notify_one();
}

void Messenger::socket_pub_send(RawMessage* raw_message) {
    zmq::message_t message = raw_message->pack_zmq_msg();
    pub_socket_.send(message);
}

void Messenger::handle_recv_message(RawMessage* raw_message) {
    if (raw_message == nullptr) return;

    switch (static_cast<MessageType>raw_message->msg_type) {
        case MessageType.UNKNOWN_REQUEST:
            return;
        case MessageType.HEARTBEAT:
            dispatch<HeartBeat>(raw_message);
            break;
        case MessageType.ACTION_REQUEST:
            dispatch<ActionRequest>(raw_message);
            break;
        case MessageType.CONNECT_REQUEST:
            dispatch<ConnectRequest>(raw_message);
            break;
        case MessageType.SHOW_DOWN_REQUEST:
            dispatch<ShowDownRequest>(raw_message);
            break;
        default:
            break;
    }
}

} // namespace server
} // namespace texas_code
