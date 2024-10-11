//
// Created by PinkySmile on 09/10/24.
//

#include <windows.h>
#include "giuroll.hpp"

namespace GiuRoll {
	static HMODULE module = nullptr;
	static void (*set_char_data_size)(size_t);
	static void (*set_char_data_pos)(size_t id, size_t , size_t);

	void load()
	{
		module = LoadLibraryA("giuroll");
		if (!module)
			return;

		set_char_data_size = reinterpret_cast<void (*)(size_t)>(GetProcAddress(module, "set_char_data_size"));
		set_char_data_pos = reinterpret_cast<void (*)(size_t, size_t, size_t)>(GetProcAddress(module, "set_char_data_pos"));

		if (set_char_data_size && set_char_data_pos)
			return;

		MessageBoxA(
			nullptr,
			"Netplay rollback not supported. This mod supports giuroll 0.6.12+, which wasn't found.\n"
			"A different (and not supported) giuroll version is in use. Please use version 0.6.12 or greater, or otherwise one that is compatible.\n"
			"Playing online now will cause desyncs",
			"CustomWeathers",
			MB_ICONWARNING | MB_OK
		);
		FreeLibrary(module);
		module = nullptr;
	}

	bool isLoaded()
	{
		return module != nullptr;
	}

	void setCharDataSize(size_t size)
	{
		if (!isLoaded())
			return;
		set_char_data_size(size);
	}

	void setCharDataPos(size_t id, size_t chrSize, size_t objSize)
	{
		if (!isLoaded())
			return;
		set_char_data_pos(id, chrSize, objSize);
	}
}