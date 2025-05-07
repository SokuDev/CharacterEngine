// Version 0.8

#include "SokuLib.hpp"
#include "Orin.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Orin::buildCharacter()\n");
	return new Orin(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Orin::getCharacterSize()\n");
	return sizeof(Orin);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Orin::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectChirno);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Orin::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Orin::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Orin::unhook();
	return TRUE;
}
