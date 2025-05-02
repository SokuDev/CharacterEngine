#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Satori.hpp"


#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

static_assert(sizeof(AppliedPatch) == 8);

GameObjectList_Satori::GameObjectList_Satori(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Yuyuko(player)
{}

void GameObjectList_Satori::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Satori::update()");
	SokuLib::v2::GameObjectList_Yuyuko::update();
	puts("<- GameObjectList_Satori::update()");
}

SokuLib::v2::GameObject *GameObjectList_Satori::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Satori::createObject()");
	auto addr = ((void **)0x85D4F4)[1];
	auto fct = SokuLib::union_cast<SokuLib::v2::GameObject*(VTableGameObjectList::*)(SokuLib::v2::GameObject*, SokuLib::v2::Player*, short, float, float, char, char, void*, unsigned int)>(addr);
	SokuLib::v2::GameObject *object = nullptr;

	__asm PUSH EBP
	(this->*fct)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	__asm POP EBP
	__asm MOV [object], EAX
	puts("<- GameObjectList_Satori::createObject()");
	return object;
}


void Satori::hook()
{
	initPatches();
}

void Satori::unhook()
{
	clearPatches();
}


Satori::Satori(SokuLib::PlayerInfo &info) :
	SokuLib::v2::PlayerYuyuko(info)
{
	this->objectList = new GameObjectList_Satori(this);
}

void Satori::update()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(compiledPatches));
	for (auto &patch : compiledPatches)
		appliedPatches.emplace_back(&patch);
	puts("-> Satori::update()");
	SokuLib::v2::PlayerYuyuko::update();
	puts("<- Satori::update()");
}

void Satori::initializeAction()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::initializeAction()");
	SokuLib::v2::PlayerYuyuko::initializeAction();
	puts("<- Satori::initializeAction()");
}

void Satori::initialize()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::initialize()");
	SokuLib::v2::PlayerYuyuko::initialize();
	puts("<- Satori::initialize()");
}

void Satori::handleInputs()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::handleInputs()");
	SokuLib::v2::PlayerYuyuko::handleInputs();
	puts("<- Satori::handleInputs()");
}

void Satori::VUnknown58()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::VUnknown58()");
	SokuLib::v2::PlayerYuyuko::VUnknown58();
	puts("<- Satori::VUnknown58()");
}

void Satori::VUnknown5C()
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::VUnknown5C()");
	SokuLib::v2::PlayerYuyuko::VUnknown5C();
	puts("<- Satori::VUnknown5C()");
}

bool Satori::VUnknown60(int a)
{
	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::VUnknown60()");
	auto b = SokuLib::v2::PlayerYuyuko::VUnknown60(a);
	puts("<- Satori::VUnknown60()");
	return b;
}
