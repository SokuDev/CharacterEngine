//
// Created by PinkySmile on 06/10/24.
//

#include "SokuLib.hpp"
#include "Tewi.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Tewi::buildCharacter()\n");
	return new Tewi(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Tewi::getCharacterSize()\n");
	return sizeof(Tewi);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Tewi::getObjectSize()\n");
	return sizeof(BasicObject);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("Tewi::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Tewi::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Tewi::unhook();
	return TRUE;
}