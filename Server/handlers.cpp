#include <handlers.hpp>
#include <vector>
#include <files.hpp>
#include <unordered_map>
#include <workers.hpp>

bool handlers::onHeartbeat(packet_heartbeat_input_t* input, packet_heartbeat_output_t** output) {
    printf("on heartbeat! [%i, %i]\n", input->status._internal, input->status.kernel);
    return true;
}

bool handlers::onShellcode(packet_shellcode_input_t* input, packet_shellcode_output_t** output) {
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

bool handlers::Dispatcher(tcp::socket& socket) {
    std::unordered_map<packet_type_e, packet_handler_t> g_handlers = {
        { packet_type_e::none, { false, false, nullptr } },
        { packet_type_e::heartbeat, { true, false, (void*)&handlers::onHeartbeat } },
        { packet_type_e::shellcode, { true, true, (void*)&handlers::onShellcode } },
    };

    request_header_t request_header;
    boost::asio::read(socket, boost::asio::buffer(&request_header, sizeof(request_header_t)));

    if (g_handlers.find(request_header.type) == g_handlers.end()) {
        return false;
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

    return true;
}