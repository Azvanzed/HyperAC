#include <server.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;


/* added tries cuz sometimes this crashes randomly */
bool server::sendServer(request_header_t* request, response_header_t** response) {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::socket socket(io_service);

    try {
        boost::asio::connect(socket, resolver.resolve({ SERVER_HOST, SERVER_PORT }));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }

    try {
        boost::asio::write(socket, boost::asio::buffer(request, request->size));

        if (response) {
            response_header_t header;
            boost::asio::read(socket, boost::asio::buffer(&header, sizeof(response_header_t)));
            *response = (response_header_t*)malloc(header.size);
            **response = header;

            void* next_buffer = (void*)(((uint64_t)*response) + sizeof(response_header_t));
            size_t next_size = header.size - sizeof(response_header_t);
            boost::asio::read(socket, boost::asio::buffer(next_buffer, next_size));
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        socket.close();
        if (response && *response) {
            free(*response);
            *response = nullptr;
        }

        return false;
    }

    socket.close();
    return true;
}

bool server::sendHeartbeat(bool _internal, bool kernel) {
	packet_heartbeat_input_t heartbeat;;
	heartbeat.size = sizeof(packet_heartbeat_input_t);
	heartbeat.type = packet_type_e::heartbeat;
	heartbeat.status._internal = _internal;
	heartbeat.status.kernel = kernel;
	return server::sendServer(&heartbeat);
}

bool server::streamShellcode(shellcode_type_e type, std::vector<uint8_t>* binary) {
	packet_shellcode_input_t shellcode = { sizeof(packet_shellcode_input_t) };
	shellcode.type = packet_type_e::shellcode;
	shellcode.shellcode = shellcode_type_e::main;

	packet_shellcode_output_t* output = nullptr;
	if (!server::sendServer(&shellcode, (response_header_t**)&output) || output->length < 0x1000) {
		return false;
	}

	for (uint16_t i = 0; i < output->length; ++i) {
		binary->push_back(output->data[i]);
	}

	return true;
}