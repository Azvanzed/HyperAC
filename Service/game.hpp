#pragma once

#include <cstdint>
#include <vector>
#include <mutex>

namespace game {
inline uint64_t g_process_id;

inline std::vector<on_image_load_t> g_images;
inline std::vector<on_thread_creation_t> g_threads;
inline std::vector<on_handle_request_t> g_handles;
inline std::vector<on_integrity_violation_t> g_integrity_violations;
}