// Version 0.8

#include "SokuLib.hpp"
#include "Momiji.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Momiji::buildCharacter()\n");
	return new Momiji(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Momiji::getCharacterSize()\n");
	return sizeof(Momiji);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Momiji::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectReimu);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Momiji::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Momiji::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Momiji::unhook();
	return TRUE;
}
