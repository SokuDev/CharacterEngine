// Version 0.7

#include "SokuLib.hpp"
#include "Sekibanki.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Sekibanki::buildCharacter()\n");
	return new Sekibanki(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Sekibanki::getCharacterSize()\n");
	return sizeof(Sekibanki);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Sekibanki::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectYoumu);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Sekibanki::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Sekibanki::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Sekibanki::unhook();
	return TRUE;
}
