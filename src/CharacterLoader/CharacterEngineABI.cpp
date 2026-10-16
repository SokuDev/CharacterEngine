//
// Created by PinkySmile on 02/03/2026.
//

#include <map>
#include <ranges>
#include "./CharacterEngineABI.hpp"

#include "checkSoku2.hpp"

std::vector<CharacterModule> modules;
std::map<void *, std::shared_ptr<CharacterModule>> loadedModules;
static std::array<size_t, 22> baseCharacterSize = {
	sizeof(SokuLib::v2::PlayerReimu),
	sizeof(SokuLib::v2::PlayerMarisa),
	sizeof(SokuLib::v2::PlayerSakuya),
	sizeof(SokuLib::v2::PlayerAlice),
	sizeof(SokuLib::v2::PlayerPatchouli),
	sizeof(SokuLib::v2::PlayerYoumu),
	sizeof(SokuLib::v2::PlayerRemilia),
	sizeof(SokuLib::v2::PlayerYuyuko),
	sizeof(SokuLib::v2::PlayerYukari),
	sizeof(SokuLib::v2::PlayerSuika),
	sizeof(SokuLib::v2::PlayerUdonge),
	sizeof(SokuLib::v2::PlayerAya),
	sizeof(SokuLib::v2::PlayerKomachi),
	sizeof(SokuLib::v2::PlayerIku),
	sizeof(SokuLib::v2::PlayerTenshi),
	sizeof(SokuLib::v2::PlayerSanae),
	sizeof(SokuLib::v2::PlayerChirno),
	sizeof(SokuLib::v2::PlayerMeirin),
	sizeof(SokuLib::v2::PlayerUtsuho),
	sizeof(SokuLib::v2::PlayerSuwako),
	0,
	sizeof(SokuLib::v2::PlayerNamazu),
};
static std::array<size_t, 22> baseObjectSize = {
	sizeof(SokuLib::v2::GameObjectReimu),
	sizeof(SokuLib::v2::GameObjectMarisa),
	sizeof(SokuLib::v2::GameObjectSakuya),
	sizeof(SokuLib::v2::GameObjectAlice),
	sizeof(SokuLib::v2::GameObjectPatchouli),
	sizeof(SokuLib::v2::GameObjectYoumu),
	sizeof(SokuLib::v2::GameObjectRemilia),
	sizeof(SokuLib::v2::GameObjectYuyuko),
	sizeof(SokuLib::v2::GameObjectYukari),
	sizeof(SokuLib::v2::GameObjectSuika),
	sizeof(SokuLib::v2::GameObjectUdonge),
	sizeof(SokuLib::v2::GameObjectAya),
	sizeof(SokuLib::v2::GameObjectKomachi),
	sizeof(SokuLib::v2::GameObjectIku),
	sizeof(SokuLib::v2::GameObjectTenshi),
	sizeof(SokuLib::v2::GameObjectSanae),
	sizeof(SokuLib::v2::GameObjectChirno),
	sizeof(SokuLib::v2::GameObjectMeirin),
	sizeof(SokuLib::v2::GameObjectUtsuho),
	sizeof(SokuLib::v2::GameObjectSuwako),
	0,
	sizeof(SokuLib::v2::GameObjectNamazu),
};


size_t getCharacterSize(int id)
{
	assert_exp_ma(id != INT32_MIN);
	assert_exp_ma(id != SokuLib::CHARACTER_RANDOM);
	if (id >= 0 && id < baseCharacterSize.size())
		return baseCharacterSize[id];
	if (id < 0) {
		if (-id > modules.size())
			return 0;
		id = modules[-id - 1].getIndex();
	}
	for (auto &module: loadedModules | std::views::values)
		if (module->getIndex() == id)
			return module->getCharacterSize();
	assert_exp_ma(false);
}

size_t getObjectSize(int id)
{
	assert_exp_ma(id != INT32_MIN);
	assert_exp_ma(id != SokuLib::CHARACTER_RANDOM);
	if (id >= 0 && id < baseObjectSize.size())
		return baseObjectSize[id];
	if (id < 0) {
		if (-id > modules.size())
			return 0;
		id = modules[-id - 1].getIndex();
	}
	for (auto &module: loadedModules | std::views::values)
		if (module->getIndex() == id)
			return module->getObjectSize();
	assert_exp_ma(false);
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