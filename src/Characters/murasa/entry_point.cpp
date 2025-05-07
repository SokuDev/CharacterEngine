// Version 0.8

#include "SokuLib.hpp"
#include "Murasa.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Murasa::buildCharacter()\n");
	return new Murasa(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Murasa::getCharacterSize()\n");
	return sizeof(Murasa);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Murasa::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectSuika);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Murasa::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Murasa::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Murasa::unhook();
	return TRUE;
}
