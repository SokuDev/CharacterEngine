#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Shou.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Shou::GameObjectList_Shou(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Tenshi(player)
{}

void GameObjectList_Shou::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Shou::update()");
	SokuLib::v2::GameObjectList_Tenshi::update();
	puts("<- GameObjectList_Shou::update()");
}

SokuLib::v2::GameObject *GameObjectList_Shou::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Shou::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Tenshi::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Shou::createObject()");
	return obj;
}


void Shou::hook()
{
	initPatches();
}

void Shou::unhook()
{
	clearPatches();
}


Shou::Shou(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerTenshi(info)
{
	this->objectList = new GameObjectList_Shou(this);
}

void Shou::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::update()");
	SokuLib::v2::PlayerTenshi::update();
	puts("<- Shou::update()");
}

void Shou::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::initializeAction()");
	SokuLib::v2::PlayerTenshi::initializeAction();
	puts("<- Shou::initializeAction()");
}

void Shou::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::initialize()");
	SokuLib::v2::PlayerTenshi::initialize();
	puts("<- Shou::initialize()");
}

void Shou::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::handleInputs()");
	SokuLib::v2::PlayerTenshi::handleInputs();
	puts("<- Shou::handleInputs()");
}

void Shou::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::VUnknown58()");
	SokuLib::v2::PlayerTenshi::VUnknown58();
	puts("<- Shou::VUnknown58()");
}

void Shou::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::VUnknown5C()");
	SokuLib::v2::PlayerTenshi::VUnknown5C();
	puts("<- Shou::VUnknown5C()");
}

bool Shou::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Shou::VUnknown60()");
	auto b = SokuLib::v2::PlayerTenshi::VUnknown60(a);
	puts("<- Shou::VUnknown60()");
	return b;
}
