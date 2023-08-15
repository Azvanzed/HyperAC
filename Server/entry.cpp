#include <iostream>
#include <cstdint>
#include <boost/asio.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <db.hpp>
#include <workers.hpp>
#include <unistd.h>
#include <files.hpp>

#include <packets.hpp>

using boost::asio::ip::tcp;


bool onHeartbeat(packet_heartbeat_input_t* input, packet_heartbeat_output_t** output) {
    printf("on heartbeat! [%i, %i]\n", input->status.dll, input->status.sys);
    return true;
}

bool onShellcode(packet_shellcode_input_t* input, packet_shellcode_output_t** output) {
    std::string filepath = "./shellcodes/";
    switch (input->shellcode) {
    case shellcode_type_e::main: {
        filepath += "HyperAC.dll";
    } break;
    case shellcode_type_e::custom_0: {
        filepath += "MessageBox.dll";
    } break;
    default:
        return false;
    }

    std::vector<uint8_t> buffer;
    if (!files::Read(filepath, &buffer)) {
        return false;
    }

    size_t size = sizeof(packet_shellcode_output_t) + buffer.size();
    *output = (packet_shellcode_output_t*)malloc(size);
    (*output)->size = size;
    (*output)->length = buffer.size();
    for (size_t i = 0; i < buffer.size(); ++i) {
        (*output)->data[i] = buffer[i];
    }

    return true;
}


struct packet_handler_t {
    bool has_input;
    bool has_output;
    void* handler;
};

std::unordered_map<packet_type_e, packet_handler_t> g_handlers = {
    { packet_type_e::none, { false, false, nullptr } },
    { packet_type_e::heartbeat, { true, false, (void*)&onHeartbeat } },
    { packet_type_e::shellcode, { true, true, (void*)&onShellcode } },
};

void acceptRequest(tcp::acceptor& acceptor) {
    acceptor.async_accept([&](const boost::system::error_code&, tcp::socket socket) {

        request_header_t request_header;
        boost::asio::read(socket, boost::asio::buffer(&request_header, sizeof(request_header_t)));

        {
            if (g_handlers.find(request_header.type) == g_handlers.end()) {
                goto Next;
            }

            packet_handler_t packet = g_handlers[request_header.type];

            request_header_t* input = nullptr;
            if (packet.has_input) {
                input = (request_header_t*)malloc(packet.has_input);
                *input = request_header;

                void* next_buffer = (void*)((uint64_t)input + sizeof(request_header_t));
                size_t next_size = input->size - sizeof(request_header_t);
                boost::asio::read(socket, boost::asio::buffer(next_buffer, next_size));
            }

            response_header_t* output = nullptr;
            workers::queueWork(packet.handler, input, packet.has_input ? input->size : 0,
                packet.has_output ? (void**)&output : nullptr);

            if (packet.has_output) {
                boost::asio::write(socket, boost::asio::buffer(output, output->size));
                free(output);
            }
        }

    Next:
        socket.close();
        acceptRequest(acceptor);
        }
    );
}


int main() {
    workers::createWorkers(1);

    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8881));

    acceptRequest(acceptor);
    io_service.run();
    return 0;
}