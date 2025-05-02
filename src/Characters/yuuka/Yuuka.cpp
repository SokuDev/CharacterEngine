#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Yuuka.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Yuuka::GameObjectList_Yuuka(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Utsuho(player)
{}

void GameObjectList_Yuuka::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Yuuka::update()");
	SokuLib::v2::GameObjectList_Utsuho::update();
	puts("<- GameObjectList_Yuuka::update()");
}

SokuLib::v2::GameObject *GameObjectList_Yuuka::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Yuuka::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Utsuho::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Yuuka::createObject()");
	return obj;
}


void Yuuka::hook()
{
	initPatches();
}

void Yuuka::unhook()
{
	clearPatches();
}


Yuuka::Yuuka(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerUtsuho(info)
{
	this->objectList = new GameObjectList_Yuuka(this);
}

void Yuuka::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::update()");
	SokuLib::v2::PlayerUtsuho::update();
	puts("<- Yuuka::update()");
}

void Yuuka::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::initializeAction()");
	SokuLib::v2::PlayerUtsuho::initializeAction();
	puts("<- Yuuka::initializeAction()");
}

void Yuuka::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::initialize()");
	SokuLib::v2::PlayerUtsuho::initialize();
	puts("<- Yuuka::initialize()");
}

void Yuuka::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::handleInputs()");
	SokuLib::v2::PlayerUtsuho::handleInputs();
	puts("<- Yuuka::handleInputs()");
}

void Yuuka::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::VUnknown58()");
	SokuLib::v2::PlayerUtsuho::VUnknown58();
	puts("<- Yuuka::VUnknown58()");
}

void Yuuka::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::VUnknown5C()");
	SokuLib::v2::PlayerUtsuho::VUnknown5C();
	puts("<- Yuuka::VUnknown5C()");
}

bool Yuuka::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Yuuka::VUnknown60()");
	auto b = SokuLib::v2::PlayerUtsuho::VUnknown60(a);
	puts("<- Yuuka::VUnknown60()");
	return b;
}
