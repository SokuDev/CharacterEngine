//
// Created by PinkySmile on 06/10/24.
//


#include <SokuLib.hpp>
#include <shlwapi.h>
#include <filesystem>

std::wstring GetLastErrorAsString(DWORD errorMessageID = GetLastError())
{
	if (errorMessageID == 0)
		return L"No Error";

	LPWSTR messageBuffer = nullptr;
	std::wstring m;

	printf("Finding message for error %lx\n", errorMessageID);
	if (FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMessageID,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPWSTR)&messageBuffer,
		0,
		nullptr
	)) {
		m = messageBuffer;
		LocalFree(messageBuffer);
		return m;
	}
	printf("Error %lx in FormatMessageW (EN)\n", GetLastError());

	if (!FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMessageID,
		0,
		(LPWSTR)&messageBuffer,
		0,
		nullptr
	)) {
		printf("Error %lx in FormatMessageW (LOCAL)\n", GetLastError());
		return L"(null)\n";
	}
	m = messageBuffer;
	LocalFree(messageBuffer);
	return m;
}

// We check if the game version is what we target (in our case, Soku 1.10a).
extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return memcmp(hash, SokuLib::targetHash, sizeof(SokuLib::targetHash)) == 0;
}

void restoreSoku2Files(const std::filesystem::path &folder)
{
	auto luaPathTemplate = folder / "config" / "SOKU2_base.lua";
	auto luaPath = folder / "config" / "SOKU2.lua";
	auto chrInfoPathTemplate = folder / "config" / "info" / "characters_base.csv";
	auto chrInfoPath = folder / "config" / "info" / "characters.csv";
	std::error_code e;

	if (!std::filesystem::exists(luaPathTemplate) || !std::filesystem::exists(chrInfoPathTemplate))
		return;

	std::filesystem::remove(luaPath, e);
	std::filesystem::remove(chrInfoPath, e);
	try {
		std::filesystem::copy(luaPathTemplate, luaPath);
	} catch (std::exception &e) {
		MessageBoxA(nullptr, e.what(), "Soku2 file copy failed", MB_ICONWARNING);
	}
	try {
		std::filesystem::copy(chrInfoPathTemplate, chrInfoPath);
	} catch (std::exception &e) {
		MessageBoxA(nullptr, e.what(), "Soku2 file copy failed", MB_ICONWARNING);
	}
}

// Called when the mod loader is ready to initialize this module.
// All hooks should be placed here. It's also a good moment to load settings from the ini.
extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	wchar_t profilePath[MAX_PATH];

	GetModuleFileNameW(hMyModule, profilePath, 1024);
	PathRemoveFileSpecW(profilePath);
	restoreSoku2Files(profilePath);

	const wchar_t *key[] = {
		L"CharacterEngine",
		L"MemoryPatch",
		L"Soku2"
	};
	wchar_t parentPath[MAX_PATH];
	wchar_t iniPath[MAX_PATH];
	wchar_t moduleFile[MAX_PATH];
	wchar_t modulePath[1024];
	HMODULE module[sizeof(key) / sizeof(*key)];

	GetModuleFileNameW(hMyModule, parentPath, MAX_PATH);
	PathRemoveFileSpecW(parentPath);
	wcscpy(iniPath, parentPath);
	PathAppendW(iniPath, L"Soku2Loader.ini");

	for (size_t i = 0; i < std::size(key); i++) {
		GetPrivateProfileStringW(L"Module", key[i], L"", moduleFile, sizeof(moduleFile) / sizeof(*moduleFile), iniPath);
		if (!*moduleFile)
			continue;

		if (PathIsRelativeW(moduleFile)) {
			wcscpy(modulePath, parentPath);
			PathAppendW(modulePath, moduleFile);
		} else
			wcscpy(modulePath, moduleFile);

		module[i] = LoadLibraryW(modulePath);
		if (!module[i]) {
			for (int j = 0; j < i; j++)
				FreeLibrary(module[j]);
			MessageBoxW(nullptr, (L"Failed to load Soku2: " + std::wstring(modulePath) + L": " + GetLastErrorAsString()).c_str(), L"Soku2 loading error", MB_ICONERROR);
			return false;
		}

		auto check = (bool (*)(const BYTE hash[16]))GetProcAddress(module[i], "CheckVersion");
		if (!check) {
			for (int j = 0; j <= i; j++)
				FreeLibrary(module[j]);
			MessageBoxW(nullptr, (L"Failed to load Soku2: " + std::wstring(modulePath) + L": Failed to get CheckVersion: " + GetLastErrorAsString()).c_str(), L"Soku2 loading error", MB_ICONERROR);
			return false;
		}
		if (!check(SokuLib::targetHash)) {
			for (int j = 0; j <= i; j++)
				FreeLibrary(module[j]);
			MessageBoxW(nullptr, (L"Failed to load Soku2: " + std::wstring(modulePath) + L": CheckVersion failed").c_str(), L"Soku2 loading error", MB_ICONERROR);
			return false;
		}

		auto init = (bool (*)(HMODULE hMyModule, HMODULE hParentModule))GetProcAddress(module[i], "Initialize");
		if (!init) {
			for (int j = 0; j <= i; j++)
				FreeLibrary(module[j]);
			MessageBoxW(nullptr, (L"Failed to load Soku2: " + std::wstring(modulePath) + L": Failed to get Initialize: " + GetLastErrorAsString()).c_str(), L"Soku2 loading error", MB_ICONERROR);
			return false;
		}
		if (!init(module[i], hMyModule)) {
			for (int j = 0; j <= i; j++)
				FreeLibrary(module[j]);
			MessageBoxW(nullptr, (L"Failed to load Soku2: " + std::wstring(modulePath) + L": Initialize failed").c_str(), L"Soku2 loading error", MB_ICONERROR);
			return false;
		}
	}
	return true;
}

extern "C" __declspec(dllexport) int getPriority()
{
	return 10000;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}