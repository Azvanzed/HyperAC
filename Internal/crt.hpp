#pragma once

#include <cstdint>

namespace crt {
	void memcpy(void* dst, void const* src, size_t size);
	void memset(void* dst, uint8_t value, size_t size);
	int32_t memcmp(const void* s1, const void* s2, size_t      n);
	int32_t wcscmp(const wchar_t* a, const wchar_t* b);
	int32_t strcmp(const char* a, const char* b);
	int32_t wcsicmp(const wchar_t* a, const wchar_t* b);
	int32_t stricmp(const char* a, const char* b);
	int32_t wcslen(const wchar_t* str);
	int32_t strlen(const char* str);
	void strcpy(char* a, const char* b);
}