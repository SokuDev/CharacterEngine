// Version 0.7

#include "SokuLib.hpp"
#include "Clownpiece.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Clownpiece::buildCharacter()\n");
	return new Clownpiece(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Clownpiece::getCharacterSize()\n");
	return sizeof(Clownpiece);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Clownpiece::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectUdonge);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Clownpiece::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Clownpiece::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Clownpiece::unhook();
	return TRUE;
}
