//
// Created by PinkySmile on 09/10/24.
//

#include <windows.h>
#include "error.hpp"

std::string GetLastErrorAsString(DWORD errorMessageID)
{
	if (errorMessageID == 0)
		return "No Error";

	LPSTR messageBuffer = nullptr;
	std::string m;

	printf("Finding message for error %lx\n", errorMessageID);
	if (FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMessageID,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPSTR)&messageBuffer,
		0,
		nullptr
	)) {
		m = messageBuffer;
		LocalFree(messageBuffer);
		return m;
	}
	printf("Error %lx in FormatMessageW (EN)\n", GetLastError());

	if (!FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMessageID,
		0,
		(LPSTR)&messageBuffer,
		0,
		nullptr
	)) {
		printf("Error %lx in FormatMessageW (LOCAL)\n", GetLastError());
		return "(null)";
	}
	m = messageBuffer;
	LocalFree(messageBuffer);
	return m;
}

std::wstring GetLastErrorAsWString(DWORD errorMessageID)
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