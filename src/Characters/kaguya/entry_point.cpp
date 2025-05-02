// Version 0.7

#include "SokuLib.hpp"
#include "Kaguya.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Kaguya::buildCharacter()\n");
	return new Kaguya(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Kaguya::getCharacterSize()\n");
	return sizeof(Kaguya);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Kaguya::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectPatchouli);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Kaguya::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Kaguya::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Kaguya::unhook();
	return TRUE;
}
