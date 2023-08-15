#pragma once

#include <cstdint>
#include <vector.hpp>

namespace integrity {
    struct integrity_t {
        char filepath[256];
        uint32_t file_hash;
        uint32_t hash;
        vector<patch_t> patches;
    };

    enum integrity_status_e : uint8_t {
        integrity_failed,
        integrity_corrupted,
        integrity_valid
    };

    integrity_status_e verifyModule(uint64_t base, integrity_t* integrity);
}


