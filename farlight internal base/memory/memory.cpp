#include <Windows.h>
#include <vector>
#include "../math/xor.h"


typedef struct _IMAGE_DOS_HEADER_ {     
	WORD   e_magic;                     
	WORD   e_cblp;                     
	WORD   e_cp;                       
	WORD   e_crlc;                  
	WORD   e_cparhdr;                  
	WORD   e_minalloc;                 
	WORD   e_maxalloc;                 
	WORD   e_ss;                       
	WORD   e_sp;                        
	WORD   e_csum;                     
	WORD   e_ip;                        
	WORD   e_cs;                     
	WORD   e_lfarlc;                  
	WORD   e_ovno;                    
	WORD   e_res[4];                   
	WORD   e_oemid;                    
	WORD   e_oeminfo;                   
	WORD   e_res2[10];                  
	LONG   e_lfanew;                    
} IMAGE_DOS_HEADER_, * PIMAGE_DOS_HEADER_;

uintptr_t sig_scan(const char* module, const char* pattern)
{
	uintptr_t moduleAdress = 0;
	moduleAdress = (uintptr_t)GetModuleHandleA(module);

	static auto patternToByte = [](const char* pattern)
	{
		auto       bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else { bytes.push_back(strtoul(current, &current, 16)); }
		}
		return bytes;
	};

	const auto dosHeader = (PIMAGE_DOS_HEADER_)moduleAdress;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(pattern);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
	}
	return NULL;
}

bool MemCompare(const char* bData, const char* bSig, const char* szMask) {
	for (; *szMask; ++szMask, ++bData, ++bSig) {
		if (*szMask == 'x' && *bData != *bSig) {
			return false;
		}
	}
	return (*szMask == NULL);
}

uintptr_t PatternScan(const char* pattern, const char* mask, uintptr_t begin, unsigned int size)
{
	unsigned int patternLength = (unsigned int)strlen(pattern);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		if (MemCompare((const char*)(begin + i), (const char*)pattern, mask)) {
			return begin + i;
		}
	}
	return (uintptr_t)nullptr;
}


uintptr_t find_base_address(LPCSTR name) {
	auto base = (uintptr_t)GetModuleHandleA(name);
	return base;
}

auto getasynckeyState_address = sig_scan(_("GameOverlayRenderer64.dll"), _("40 53 48 83 EC 20 80 3D ? ? ? ? ? 8B D9"));
using GetAsyncKeyState_t = short(__fastcall*)(int);

short steam_keybind(const int key)
{
	auto fnGetAyncKeyState = (GetAsyncKeyState_t)getasynckeyState_address;
	return fnGetAyncKeyState(key);
}
