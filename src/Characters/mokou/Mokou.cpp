#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Mokou.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Mokou::GameObjectList_Mokou(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Aya(player)
{}

void GameObjectList_Mokou::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(compiledPatches));
	for (auto &patch : compiledPatches)
		appliedPatches.emplace_back(&patch);
	puts("-> GameObjectList_Mokou::update()");
	SokuLib::v2::GameObjectList_Aya::update();
	puts("<- GameObjectList_Mokou::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *GameObjectList_Mokou::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Mokou::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Aya::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Mokou::createObject()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return obj;
}


void Mokou::hook()
{
	initPatches();
}

void Mokou::unhook()
{
	clearPatches();
}


Mokou::Mokou(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerAya(info)
{
	this->objectList = new GameObjectList_Mokou(this);
}

void Mokou::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::update()");
	SokuLib::v2::PlayerAya::update();
	puts("<- Mokou::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Mokou::initializeAction()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::initializeAction()");
	SokuLib::v2::PlayerAya::initializeAction();
	puts("<- Mokou::initializeAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Mokou::initialize()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::initialize()");
	SokuLib::v2::PlayerAya::initialize();
	puts("<- Mokou::initialize()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Mokou::handleInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::handleInputs()");
	SokuLib::v2::PlayerAya::handleInputs();
	puts("<- Mokou::handleInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Mokou::VUnknown58()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::VUnknown58()");
	SokuLib::v2::PlayerAya::VUnknown58();
	puts("<- Mokou::VUnknown58()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Mokou::VUnknown5C()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::VUnknown5C()");
	SokuLib::v2::PlayerAya::VUnknown5C();
	puts("<- Mokou::VUnknown5C()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

bool Mokou::VUnknown60(int a)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mokou::VUnknown60()");
	auto b = SokuLib::v2::PlayerAya::VUnknown60(a);
	puts("<- Mokou::VUnknown60()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return b;
}
