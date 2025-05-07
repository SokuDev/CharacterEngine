// Version 0.8

#include "SokuLib.hpp"
#include "Yuuka.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Yuuka::buildCharacter()\n");
	return new Yuuka(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Yuuka::getCharacterSize()\n");
	return sizeof(Yuuka);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Yuuka::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectUtsuho);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Yuuka::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Yuuka::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Yuuka::unhook();
	return TRUE;
}
