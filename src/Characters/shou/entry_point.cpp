// Version 0.8

#include "SokuLib.hpp"
#include "Shou.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Shou::buildCharacter()\n");
	return new Shou(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Shou::getCharacterSize()\n");
	return sizeof(Shou);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Shou::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectTenshi);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Shou::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Shou::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Shou::unhook();
	return TRUE;
}
