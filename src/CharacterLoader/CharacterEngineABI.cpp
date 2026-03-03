//
// Created by PinkySmile on 02/03/2026.
//

#include <map>
#include <ranges>
#include "./CharacterEngineABI.hpp"

#include "checkSoku2.hpp"

std::vector<CharacterModule> modules;
std::map<void *, std::shared_ptr<CharacterModule>> loadedModules;

size_t getCharacterSize(int id)
{
	if (id == INT32_MIN)
		return 0;
	if (id < 0) {
		if (-id > modules.size())
			return 0;
		id = modules[-id - 1].getIndex();
	}
	for (auto &module: loadedModules | std::views::values)
		if (module->getIndex() == id)
			return module->getCharacterSize();
	return 0;
}

size_t getObjectSize(int id)
{
	if (id == INT32_MIN)
		return 0;
	if (id < 0) {
		if (-id > modules.size())
			return 0;
		id = modules[-id - 1].getIndex();
	}
	for (auto &module: loadedModules | std::views::values)
		if (module->getIndex() == id)
			return module->getObjectSize();
	return 0;
}

SokuLib::v2::Player *createCharacter(int id, SokuLib::PlayerInfo &info)
{
	printf("createCharacter(%i)\n", id);
	if (id >= 0 && hasSoku2) {
		puts(" -> Delegating to Soku2");
		return SokuLib::v2::GameDataManager::createPlayer(info);
	}
	if (SokuLib::CHARACTER_REIMU <= id && id <= SokuLib::CHARACTER_NAMAZU) {
		puts(" -> Delegating to the game");
		return SokuLib::v2::GameDataManager::createPlayer(info);
	}

	CharacterModule *module = nullptr;

	if (id < 0) {
		if (id != INT32_MIN && -id <= modules.size())
			module = &modules[-id - 1];
	} else for (auto &i : modules)
		if (i.getIndex() == id) {
			module = &i;
			break;
		}
	if (!module) {
		MessageBoxA(SokuLib::window, ("Fatal error in createCharacter: Cannot find character index " + std::to_string(id)).c_str(), "Fatal Error", MB_ICONERROR);
		return nullptr;
	}

	std::shared_ptr<CharacterModule> old;

	for (auto &m: loadedModules | std::views::values)
		if (module->getIndex() == m->getIndex()) {
			old = m;
			break;
		}
	while (true) {
		auto nModule = old ? &*old : new CharacterModule(*module);

		try {
			if (!old) {
				puts("Loading DLL");
				nModule->load();
			}

			puts("Building character");
			auto chr = nModule->build(info);

			if (!chr)
				throw std::runtime_error("Build character returned null");
			if (!old) {
				puts("Reset and register");
				loadedModules[chr].reset(nModule);
				nModule->registerGR();
			} else {
				puts("Copy pointer");
				loadedModules[chr] = old;
			}
			printf("Return ptr 0x%p\n", chr);
			return chr;
		} catch (std::exception &e) {
			if (!old)
				delete nModule;
			if (MessageBoxA(nullptr, ("Failed to create character: " + std::string(e.what())).c_str(), "CharacterEngine error", MB_RETRYCANCEL) == IDCANCEL)
				return nullptr;
		}
	}
}

void destroyCharacter(SokuLib::v2::Player *ptr)
{
	delete ptr;
	loadedModules.erase(ptr);
}

CharacterEngineABI abi{
	ABI_VERSION,
	ABIS_SUPPORTED,
	getCharacterSize,
	getObjectSize,
	createCharacter,
	destroyCharacter
};