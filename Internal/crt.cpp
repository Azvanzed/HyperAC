#include <crt.hpp>

#define tolower(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

void crt::memcpy(void* dst, void const* src, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        *(uint8_t*)((uint64_t)dst + i) = *(uint8_t*)((uint64_t)src + i);
    }
}


void crt::memset(void* dst, uint8_t value, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        *(uint8_t*)((uint64_t)dst + i) = value;
    }
}

int32_t crt::wcscmp(const wchar_t* a, const wchar_t* b) {
    int32_t result = 0;
    while (!(result = *a - *b) && *b) {
        ++a;
        ++b;
    }

    return result;
}

int32_t crt::strcmp(const char* a, const char* b) {
    int32_t result = 0;
    while (!(result = *a - *b) && *b) {
        ++a;
        ++b;
    }

    return result;
}


int32_t crt::wcsicmp(const wchar_t* a, const wchar_t* b) {
    int32_t result = 0;
    while (!(result = tolower(*a) - tolower(*b)) && *b) {
        ++a;
        ++b;
    }

    return result;
}

int32_t crt::stricmp(const char* a, const char* b) {
    int32_t result = 0;
    while (!(result = tolower(*a) - tolower(*b)) && *b) {
        ++a;
        ++b;
    }

    return result;
}

int32_t crt::wcslen(const wchar_t* str) {
    int32_t len = 0;
    while (*str++) {
        ++len;
    }

    return len;
}

int32_t crt::strlen(const char* str) {
    int32_t len = 0;
    while (*str++) {
        ++len;
    }

    return len;
}