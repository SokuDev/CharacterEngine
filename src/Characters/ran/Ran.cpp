#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Ran.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Ran::GameObjectList_Ran(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Sakuya(player)
{}

void GameObjectList_Ran::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Ran::update()");
	SokuLib::v2::GameObjectList_Sakuya::update();
	puts("<- GameObjectList_Ran::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *GameObjectList_Ran::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Ran::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Sakuya::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Ran::createObject()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return obj;
}


void Ran::hook()
{
	initPatches();
}

void Ran::unhook()
{
	clearPatches();
}


Ran::Ran(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerSakuya(info)
{
	this->objectList = new GameObjectList_Ran(this);
}

void Ran::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::update()");
	SokuLib::v2::PlayerSakuya::update();
	puts("<- Ran::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Ran::initializeAction()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::initializeAction()");
	SokuLib::v2::PlayerSakuya::initializeAction();
	puts("<- Ran::initializeAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Ran::initialize()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::initialize()");
	SokuLib::v2::PlayerSakuya::initialize();
	puts("<- Ran::initialize()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Ran::handleInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::handleInputs()");
	SokuLib::v2::PlayerSakuya::handleInputs();
	puts("<- Ran::handleInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Ran::VUnknown58()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::VUnknown58()");
	SokuLib::v2::PlayerSakuya::VUnknown58();
	puts("<- Ran::VUnknown58()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Ran::VUnknown5C()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::VUnknown5C()");
	SokuLib::v2::PlayerSakuya::VUnknown5C();
	puts("<- Ran::VUnknown5C()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

bool Ran::VUnknown60(int a)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Ran::VUnknown60()");
	auto b = SokuLib::v2::PlayerSakuya::VUnknown60(a);
	puts("<- Ran::VUnknown60()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return b;
}
