#pragma once
#include <cstdint>

extern uintptr_t sig_scan(const char* module, const char* pattern);
extern uintptr_t PatternScan(const char* pattern, const char* mask, uintptr_t begin, unsigned int size);
extern uintptr_t find_base_address(LPCSTR name);
extern short steam_keybind(const int key);

template<typename T = uintptr_t, typename A>
T& read(A address) {
	auto addresss = (DWORD64)address;
	if (addresss > 0x40000 && (addresss + sizeof(T)) < 0x7FFFFFFF0000)
		return *(T*)addresss;

	auto n = nullptr;
	return reinterpret_cast<T&>(n);
}


template <typename T>
void write(uintptr_t address, T data) {
	if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000)
		*(T*)(address) = data;
}