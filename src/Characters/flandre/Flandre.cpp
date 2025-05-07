#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Flandre.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Flandre::GameObjectList_Flandre(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Remilia(player)
{}

void GameObjectList_Flandre::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Flandre::update()");
	SokuLib::v2::GameObjectList_Remilia::update();
	puts("<- GameObjectList_Flandre::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *GameObjectList_Flandre::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Flandre::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Remilia::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Flandre::createObject()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return obj;
}


void Flandre::hook()
{
	initPatches();
}

void Flandre::unhook()
{
	clearPatches();
}


Flandre::Flandre(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerRemilia(info)
{
	this->objectList = new GameObjectList_Flandre(this);
}

void Flandre::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::update()");
	SokuLib::v2::PlayerRemilia::update();
	puts("<- Flandre::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Flandre::initializeAction()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::initializeAction()");
	SokuLib::v2::PlayerRemilia::initializeAction();
	puts("<- Flandre::initializeAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Flandre::initialize()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::initialize()");
	SokuLib::v2::PlayerRemilia::initialize();
	puts("<- Flandre::initialize()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Flandre::handleInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::handleInputs()");
	SokuLib::v2::PlayerRemilia::handleInputs();
	puts("<- Flandre::handleInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Flandre::VUnknown58()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::VUnknown58()");
	SokuLib::v2::PlayerRemilia::VUnknown58();
	puts("<- Flandre::VUnknown58()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Flandre::VUnknown5C()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::VUnknown5C()");
	SokuLib::v2::PlayerRemilia::VUnknown5C();
	puts("<- Flandre::VUnknown5C()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

bool Flandre::VUnknown60(int a)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Flandre::VUnknown60()");
	auto b = SokuLib::v2::PlayerRemilia::VUnknown60(a);
	puts("<- Flandre::VUnknown60()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return b;
}
