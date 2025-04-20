// Version {{MAJOR}}.{{MINOR}}

#include "SokuLib.hpp"
#include "{{ClassName}}.hpp"
#include "BasicObject.hpp"

extern "C" __declspec(dllexport) SokuLib::v2::Player *buildCharacter(SokuLib::PlayerInfo &p)
{
	printf("{{ClassName}}::buildCharacter()\n");
	return new {{ClassName}}(p);
}

extern "C" __declspec(dllexport) size_t getCharacterSize()
{
	printf("{{ClassName}}::getCharacterSize()\n");
	return sizeof({{ClassName}});
}

extern "C" __declspec(dllexport) size_t getObjectSize()
{
	printf("{{ClassName}}::getObjectSize()\n");
	return sizeof(SokuLib::v2::GameObject{{BaseName}});
}

extern "C" __declspec(dllexport) int APIENTRY DllMain(HMODULE, DWORD fdwReason, LPVOID)
{
	printf("{{ClassName}}::DllMain(%lu)\n", fdwReason);
	if (fdwReason == DLL_PROCESS_ATTACH)
		{{ClassName}}::hook();
	if (fdwReason == DLL_PROCESS_DETACH)
		{{ClassName}}::unhook();
	return TRUE;
}
