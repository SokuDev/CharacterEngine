#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Murasa.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Murasa::GameObjectList_Murasa(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Suika(player)
{}

void GameObjectList_Murasa::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Murasa::update()");
	SokuLib::v2::GameObjectList_Suika::update();
	puts("<- GameObjectList_Murasa::update()");
}

SokuLib::v2::GameObject *GameObjectList_Murasa::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Murasa::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Suika::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Murasa::createObject()");
	return obj;
}


void Murasa::hook()
{
	initPatches();
}

void Murasa::unhook()
{
	clearPatches();
}


Murasa::Murasa(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerSuika(info)
{
	this->objectList = new GameObjectList_Murasa(this);
}

void Murasa::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::update()");
	SokuLib::v2::PlayerSuika::update();
	puts("<- Murasa::update()");
}

void Murasa::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::initializeAction()");
	SokuLib::v2::PlayerSuika::initializeAction();
	puts("<- Murasa::initializeAction()");
}

void Murasa::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::initialize()");
	SokuLib::v2::PlayerSuika::initialize();
	puts("<- Murasa::initialize()");
}

void Murasa::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::handleInputs()");
	SokuLib::v2::PlayerSuika::handleInputs();
	puts("<- Murasa::handleInputs()");
}

void Murasa::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::VUnknown58()");
	SokuLib::v2::PlayerSuika::VUnknown58();
	puts("<- Murasa::VUnknown58()");
}

void Murasa::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::VUnknown5C()");
	SokuLib::v2::PlayerSuika::VUnknown5C();
	puts("<- Murasa::VUnknown5C()");
}

bool Murasa::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Murasa::VUnknown60()");
	auto b = SokuLib::v2::PlayerSuika::VUnknown60(a);
	puts("<- Murasa::VUnknown60()");
	return b;
}
