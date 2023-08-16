#pragma once

#include <cstdint>
#include <packets.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace handlers {
    struct packet_handler_t {
        bool has_input;
        bool has_output;
        void* handler;
    };

    bool onHeartbeat(packet_heartbeat_input_t* input, packet_heartbeat_output_t** output);

    bool onShellcode(packet_shellcode_input_t* input, packet_shellcode_output_t** output);

    bool Dispatcher(tcp::socket& socket);
}