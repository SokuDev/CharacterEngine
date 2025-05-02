// Version 0.7

#include "SokuLib.hpp"
#include "Ran.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Ran::buildCharacter()\n");
	return new Ran(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Ran::getCharacterSize()\n");
	return sizeof(Ran);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Ran::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectSakuya);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Ran::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Ran::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Ran::unhook();
	return TRUE;
}
