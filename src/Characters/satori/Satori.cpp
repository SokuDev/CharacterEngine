#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Satori.hpp"


#define puts(...)
#define printf(...)

GameObjectList_Satori::GameObjectList_Satori(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Yuyuko(player)
{}

void GameObjectList_Satori::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Satori::update()");
	SokuLib::v2::GameObjectList_Yuyuko::update();
	puts("<- GameObjectList_Satori::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *GameObjectList_Satori::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Satori::createObject()");
	auto addr = ((void **)0x85D4F4)[1];
	auto fct = SokuLib::union_cast<SokuLib::v2::GameObject*(VTableGameObjectList::*)(SokuLib::v2::GameObject*, SokuLib::v2::Player*, short, float, float, char, char, void*, unsigned int)>(addr);
	SokuLib::v2::GameObject *obj = nullptr;

	__asm PUSH EBP
	(this->*fct)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	__asm POP EBP
	__asm MOV [obj], EAX
	puts("<- GameObjectList_Satori::createObject()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return obj;
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
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(compiledPatches));
	for (auto &patch : compiledPatches)
		appliedPatches.emplace_back(&patch);
	puts("-> Satori::update()");
	SokuLib::v2::PlayerYuyuko::update();
	puts("<- Satori::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Satori::initializeAction()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::initializeAction()");
	SokuLib::v2::PlayerYuyuko::initializeAction();
	puts("<- Satori::initializeAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Satori::initialize()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::initialize()");
	SokuLib::v2::PlayerYuyuko::initialize();
	puts("<- Satori::initialize()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Satori::handleInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::handleInputs()");
	SokuLib::v2::PlayerYuyuko::handleInputs();
	puts("<- Satori::handleInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Satori::computerInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(computerInputs_patches));
	for (auto &patch : computerInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::computerInputs()");
	SokuLib::v2::PlayerYuyuko::computerInputs();
	puts("<- Satori::computerInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Satori::updateStory()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(updateStory_patches));
	for (auto &patch : updateStory_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::updateStory()");
	SokuLib::v2::PlayerYuyuko::updateStory();
	puts("<- Satori::updateStory()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

bool Satori::setCustomScenarioAction(int a)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(setCustomScenarioAction_patches));
	for (auto &patch : setCustomScenarioAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Satori::setCustomScenarioAction()");
	auto b = SokuLib::v2::PlayerYuyuko::setCustomScenarioAction(a);
	puts("<- Satori::setCustomScenarioAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return b;
}
