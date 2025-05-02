// Version 0.7

#include "SokuLib.hpp"
#include "Mokou.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Mokou::buildCharacter()\n");
	return new Mokou(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Mokou::getCharacterSize()\n");
	return sizeof(Mokou);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Mokou::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectAya);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Mokou::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Mokou::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Mokou::unhook();
	return TRUE;
}
