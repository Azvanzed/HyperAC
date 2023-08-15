#pragma once

#include <cstdint>

enum class packet_type_e : uint8_t {
    none,
    heartbeat,
    shellcode
};

enum class shellcode_type_e : uint8_t {
    none,
    main,
    custom_0,
};

#pragma pack(push, 1)
struct request_header_t {
    size_t size;
    packet_type_e type;
};

struct response_header_t {
    size_t size;
};

struct packet_heartbeat_input_t : request_header_t {
    struct {
        bool dll : 1;
        bool sys : 1;
    }status;
};

struct packet_heartbeat_output_t : response_header_t {
};

struct packet_shellcode_input_t : request_header_t {
    shellcode_type_e shellcode;
};

struct packet_shellcode_output_t : response_header_t {
    uint16_t length;
    uint8_t data[];
};

#pragma pack(pop)
