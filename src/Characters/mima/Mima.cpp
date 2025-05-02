#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Mima.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Mima::GameObjectList_Mima(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Iku(player)
{}

void GameObjectList_Mima::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Mima::update()");
	SokuLib::v2::GameObjectList_Iku::update();
	puts("<- GameObjectList_Mima::update()");
}

SokuLib::v2::GameObject *GameObjectList_Mima::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Mima::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Iku::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Mima::createObject()");
	return obj;
}


void Mima::hook()
{
	initPatches();
}

void Mima::unhook()
{
	clearPatches();
}


Mima::Mima(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerIku(info)
{
	this->objectList = new GameObjectList_Mima(this);
}

void Mima::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::update()");
	SokuLib::v2::PlayerIku::update();
	puts("<- Mima::update()");
}

void Mima::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::initializeAction()");
	SokuLib::v2::PlayerIku::initializeAction();
	puts("<- Mima::initializeAction()");
}

void Mima::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::initialize()");
	SokuLib::v2::PlayerIku::initialize();
	puts("<- Mima::initialize()");
}

void Mima::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::handleInputs()");
	SokuLib::v2::PlayerIku::handleInputs();
	puts("<- Mima::handleInputs()");
}

void Mima::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::VUnknown58()");
	SokuLib::v2::PlayerIku::VUnknown58();
	puts("<- Mima::VUnknown58()");
}

void Mima::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::VUnknown5C()");
	SokuLib::v2::PlayerIku::VUnknown5C();
	puts("<- Mima::VUnknown5C()");
}

bool Mima::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Mima::VUnknown60()");
	auto b = SokuLib::v2::PlayerIku::VUnknown60(a);
	puts("<- Mima::VUnknown60()");
	return b;
}
