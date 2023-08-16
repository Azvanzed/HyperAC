#include <service.hpp>

bool service::invokeRequestCallbackEx(void* buffer, size_t size) {
    void* alloc = API(VirtualAllocEx)(g_service, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc) {
        return false;
    }

    API(WriteProcessMemory)(g_service, alloc, buffer, size, nullptr);
    HANDLE hthread = API(CreateRemoteThread)(g_service, nullptr, 0, (LPTHREAD_START_ROUTINE)g_service_callback, alloc, 0, nullptr);
    if (hthread == INVALID_HANDLE_VALUE) {
        API(VirtualFreeEx)(g_service, alloc, 0, MEM_RELEASE);
        return false;
    }

    API(CloseHandle)(hthread);
    return true;
}


bool service::sendHeartbeat() {
    on_internal_heartbeat_t callback;
    callback.type = service_callback_type_e::internal_heartbeat;
    callback.hashed = ((uint64_t)g_service_callback ^ HEARTBEAT_XOR_KEY);
    return service::invokeRequestCallback(callback);
}