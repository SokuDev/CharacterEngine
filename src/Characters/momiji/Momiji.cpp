#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Momiji.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Momiji::GameObjectList_Momiji(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Reimu(player)
{}

void GameObjectList_Momiji::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Momiji::update()");
	SokuLib::v2::GameObjectList_Reimu::update();
	puts("<- GameObjectList_Momiji::update()");
}

SokuLib::v2::GameObject *GameObjectList_Momiji::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Momiji::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Reimu::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Momiji::createObject()");
	return obj;
}


void Momiji::hook()
{
	initPatches();
}

void Momiji::unhook()
{
	clearPatches();
}


Momiji::Momiji(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerReimu(info)
{
	this->objectList = new GameObjectList_Momiji(this);
}

void Momiji::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::update()");
	SokuLib::v2::PlayerReimu::update();
	puts("<- Momiji::update()");
}

void Momiji::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::initializeAction()");
	SokuLib::v2::PlayerReimu::initializeAction();
	puts("<- Momiji::initializeAction()");
}

void Momiji::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::initialize()");
	SokuLib::v2::PlayerReimu::initialize();
	puts("<- Momiji::initialize()");
}

void Momiji::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::handleInputs()");
	SokuLib::v2::PlayerReimu::handleInputs();
	puts("<- Momiji::handleInputs()");
}

void Momiji::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::VUnknown58()");
	SokuLib::v2::PlayerReimu::VUnknown58();
	puts("<- Momiji::VUnknown58()");
}

void Momiji::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::VUnknown5C()");
	SokuLib::v2::PlayerReimu::VUnknown5C();
	puts("<- Momiji::VUnknown5C()");
}

bool Momiji::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Momiji::VUnknown60()");
	auto b = SokuLib::v2::PlayerReimu::VUnknown60(a);
	puts("<- Momiji::VUnknown60()");
	return b;
}
