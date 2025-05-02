// Version 0.7

#include "SokuLib.hpp"
#include "Mima.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Mima::buildCharacter()\n");
	return new Mima(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Mima::getCharacterSize()\n");
	return sizeof(Mima);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Mima::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObjectIku);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Mima::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Mima::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Mima::unhook();
	return TRUE;
}
