#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "Kaguya.hpp"


#define puts(...)
#define printf(...)

GameObjectList_Kaguya::GameObjectList_Kaguya(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_Patchouli(player)
{}

void GameObjectList_Kaguya::update()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Kaguya::update()");
	SokuLib::v2::GameObjectList_Patchouli::update();
	puts("<- GameObjectList_Kaguya::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *GameObjectList_Kaguya::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> GameObjectList_Kaguya::createObject()");
	auto obj = SokuLib::v2::GameObjectList_Patchouli::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	puts("<- GameObjectList_Kaguya::createObject()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
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
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::update()");
	SokuLib::v2::PlayerPatchouli::update();
	puts("<- Kaguya::update()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Kaguya::initializeAction()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::initializeAction()");
	SokuLib::v2::PlayerPatchouli::initializeAction();
	puts("<- Kaguya::initializeAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Kaguya::initialize()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::initialize()");
	SokuLib::v2::PlayerPatchouli::initialize();
	puts("<- Kaguya::initialize()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Kaguya::handleInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::handleInputs()");
	SokuLib::v2::PlayerPatchouli::handleInputs();
	puts("<- Kaguya::handleInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Kaguya::computerInputs()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(computerInputs_patches));
	for (auto &patch : computerInputs_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::computerInputs()");
	SokuLib::v2::PlayerPatchouli::computerInputs();
	puts("<- Kaguya::computerInputs()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

void Kaguya::updateStory()
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(updateStory_patches));
	for (auto &patch : updateStory_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::updateStory()");
	SokuLib::v2::PlayerPatchouli::updateStory();
	puts("<- Kaguya::updateStory()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
}

bool Kaguya::setCustomScenarioAction(int a)
{
	DWORD old;

	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);

	std::vector<AppliedPatch, Allocator> appliedPatches{applyAllocator};

	appliedPatches.reserve(std::size(setCustomScenarioAction_patches));
	for (auto &patch : setCustomScenarioAction_patches)
		appliedPatches.emplace_back(patch);
	puts("-> Kaguya::setCustomScenarioAction()");
	auto b = SokuLib::v2::PlayerPatchouli::setCustomScenarioAction(a);
	puts("<- Kaguya::setCustomScenarioAction()");
	appliedPatches.clear();
	VirtualProtect(reinterpret_cast<LPVOID>(TEXT_SECTION_OFFSET), TEXT_SECTION_SIZE, old, &old);
	return b;
}
