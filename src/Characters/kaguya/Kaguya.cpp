#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Kaguya.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Kaguya::GameObjectList_Kaguya(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Patchouli(player)
{}

void GameObjectList_Kaguya::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Kaguya::update()");
	SokuLib::v2::GameObjectList_Patchouli::update();
	puts("<- GameObjectList_Kaguya::update()");
}

SokuLib::v2::GameObject *GameObjectList_Kaguya::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Kaguya::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Patchouli::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Kaguya::createObject()");
	return obj;
}


void Kaguya::hook()
{
	initPatches();
}

void Kaguya::unhook()
{
	clearPatches();
}


Kaguya::Kaguya(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerPatchouli(info)
{
	this->objectList = new GameObjectList_Kaguya(this);
}

void Kaguya::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::update()");
	SokuLib::v2::PlayerPatchouli::update();
	puts("<- Kaguya::update()");
}

void Kaguya::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::initializeAction()");
	SokuLib::v2::PlayerPatchouli::initializeAction();
	puts("<- Kaguya::initializeAction()");
}

void Kaguya::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::initialize()");
	SokuLib::v2::PlayerPatchouli::initialize();
	puts("<- Kaguya::initialize()");
}

void Kaguya::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::handleInputs()");
	SokuLib::v2::PlayerPatchouli::handleInputs();
	puts("<- Kaguya::handleInputs()");
}

void Kaguya::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::VUnknown58()");
	SokuLib::v2::PlayerPatchouli::VUnknown58();
	puts("<- Kaguya::VUnknown58()");
}

void Kaguya::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::VUnknown5C()");
	SokuLib::v2::PlayerPatchouli::VUnknown5C();
	puts("<- Kaguya::VUnknown5C()");
}

bool Kaguya::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::VUnknown60()");
	auto b = SokuLib::v2::PlayerPatchouli::VUnknown60(a);
	puts("<- Kaguya::VUnknown60()");
	return b;
}
