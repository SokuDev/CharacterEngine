// Version 0.8

#include "SokuLib.hpp"
#include "Flandre.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Flandre::buildCharacter()\n");
	return new Flandre(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Flandre::getCharacterSize()\n");
	return sizeof(Flandre);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Flandre::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectRemilia);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Flandre::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Flandre::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Flandre::unhook();
	return TRUE;
}
