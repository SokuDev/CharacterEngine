#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"
#include "{{ClassName}}.hpp"


GameObjectList_{{ClassName}}::GameObjectList_{{ClassName}}(SokuLib::v2::Player *player) :
	SokuLib::v2::GameObjectList_{{BaseName}}(player)
{}

void GameObjectList_{{ClassName}}::update()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(objectUpdate_patches) + std::size(objectInitializeAction_patches));
	for (auto &patch : objectUpdate_patches)
		appliedPatches.emplace_back(patch);
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	return SokuLib::v2::GameObjectList_{{BaseName}}::update();
}

SokuLib::v2::GameObject *GameObjectList_{{ClassName}}::createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8)
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(objectInitializeAction_patches));
	for (auto &patch : objectInitializeAction_patches)
		appliedPatches.emplace_back(patch);
	return SokuLib::v2::GameObjectList_{{BaseName}}::createObject(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}


void {{ClassName}}::hook()
{
	initPatches();
}

void {{ClassName}}::unhook()
{
	clearPatches();
}


{{ClassName}}::{{ClassName}}(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player{{BaseName}}(info)
{
	this->objectList = new GameObjectList_{{ClassName}}(this);
}

void {{ClassName}}::update()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(update_patches));
	for (auto &patch : update_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::update();
}

void {{ClassName}}::initializeAction()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(initializeAction_patches));
	for (auto &patch : initializeAction_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::initializeAction();
}

void {{ClassName}}::initialize()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(initialize_patches));
	for (auto &patch : initialize_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::initialize();
}

void {{ClassName}}::handleInputs()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(handleInputs_patches));
	for (auto &patch : handleInputs_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::handleInputs();
}

void {{ClassName}}::VUnknown58()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(VUnknown58_patches));
	for (auto &patch : VUnknown58_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::VUnknown58();
}

void {{ClassName}}::VUnknown5C()
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(VUnknown5C_patches));
	for (auto &patch : VUnknown5C_patches)
		appliedPatches.emplace_back(patch);
	SokuLib::v2::Player{{BaseName}}::VUnknown5C();
}

bool {{ClassName}}::VUnknown60(int a)
{
	std::vector<AppliedPatch> appliedPatches;

	appliedPatches.reserve(std::size(VUnknown60_patches));
	for (auto &patch : VUnknown60_patches)
		appliedPatches.emplace_back(patch);
	return SokuLib::v2::Player{{BaseName}}::VUnknown60(a);
}
