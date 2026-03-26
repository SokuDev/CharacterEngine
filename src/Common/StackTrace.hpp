//
// Created by PinkySmile on 23/03/2026.
//

#ifndef CHARACTERENGINE_STACKTRACE_HPP
#define CHARACTERENGINE_STACKTRACE_HPP


#pragma comment(lib, "dbghelp.lib")
#include <windows.h>
#include <dbghelp.h>
#include <iostream>

extern "C" USHORT WINAPI CaptureStackBackTrace(
	_In_ ULONG FramesToSkip,
	_In_ ULONG FramesToCapture,
	_Out_writes_(FramesToCapture) PVOID* BackTrace,
	_Out_opt_ PULONG BackTraceHash
);

static HMODULE _hModule = nullptr;

struct SymbolMatch {
	DWORD64 targetAddr;
	DWORD64 closestAddr;
	std::string name;
};


static void PrintStackTrace() {
	HANDLE process = GetCurrentProcess();
	char dllPath[MAX_PATH];

	GetModuleHandleExA(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&PrintStackTrace,
		&_hModule
	);
	GetModuleFileNameA(_hModule, dllPath, MAX_PATH);

	std::string pathStr = dllPath;
	std::string dir = pathStr.substr(0, pathStr.find_last_of("\\/"));

	// Initialize with verbose options
	SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);;
	if (!SymInitialize(process, dir.c_str(), FALSE)) return;

	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (GetFileAttributesExA(dllPath, GetFileExInfoStandard, &fad)) {
		SymLoadModuleEx(process, nullptr, dllPath, nullptr, reinterpret_cast<DWORD64>(_hModule), fad.nFileSizeLow, nullptr, 0);
	}

	void *stack[64];
	unsigned short frames = CaptureStackBackTrace(1, 64, stack, nullptr);
	auto symbol = static_cast<SYMBOL_INFO *>(calloc(1, sizeof(SYMBOL_INFO) + 256 * sizeof(char)));

	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	std::cout << "--- Stack Trace ---" << std::endl;

	for (unsigned int i = 0; i < frames; i++) {
		auto address = reinterpret_cast<DWORD64>(stack[i]);
		DWORD64 displacement = 0;

		if (SymFromAddr(process, address, &displacement, symbol)) {
			IMAGEHLP_LINE64 line;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD lineDisplacement;

			if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line))
				std::cout << i << ": " << symbol->Name << " [" << line.FileName << ":" << line.LineNumber << "]" << std::endl;
			else
				std::cout << i << ": " << symbol->Name << " + 0x" << std::hex << displacement << std::dec << std::endl;
		} else {
			IMAGEHLP_MODULE64 mod;
			mod.SizeOfStruct = sizeof(mod);
			if (SymGetModuleInfo64(process, address, &mod))
				std::cout << i << ": [" << mod.ModuleName << " + 0x" << std::hex << (address - mod.BaseOfImage) << "]" << std::dec << std::endl;
			else
				std::cout << i << ": Unknown (0x" << std::hex << address << ")" << std::dec << std::endl;
		}
	}
	free(symbol);
}

#endif //CHARACTERENGINE_STACKTRACE_HPP