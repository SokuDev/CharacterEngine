// Version 0.7

#include "SokuLib.hpp"
#include "Satori.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Satori::buildCharacter()\n");
	return new Satori(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Satori::getCharacterSize()\n");
	return sizeof(Satori);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Satori::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectYuyuko);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Satori::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Satori::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Satori::unhook();
	return TRUE;
}
