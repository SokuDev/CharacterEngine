//
// Created by PinkySmile on 06/10/24.
//

#include "SokuLib.hpp"
#include "Mamizou.hpp"
#include "BasicObject.hpp"
#include "CharacterEngineABI.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("Mamizou::buildCharacter()\n");
	assert_exp_mr(abiPointer, nullptr);
	assert_exp_mr(abiPointer->version >= ABI_VERSION, nullptr);
	assert_exp_mr(std::ranges::find(abiPointer->supportedVersions, ABI_VERSION) != std::end(abiPointer->supportedVersions), nullptr);
	return new Mamizou(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("Mamizou::getCharacterSize()\n");
	return sizeof(Mamizou);
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("Mamizou::getObjectSize()\n");
	return sizeof(BasicObject);
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	const char *names[] = {
		"DLL_PROCESS_DETACH",
		"DLL_PROCESS_ATTACH",
		"DLL_THREAD_ATTACH",
		"DLL_THREAD_DETACH"
	};

	printf("Mamizou::DllMain(%s)\n", names[fdwReason]);
	if (fdwReason == DLL_PROCESS_ATTACH)
		Mamizou::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		Mamizou::unhook();
	return TRUE;
}
