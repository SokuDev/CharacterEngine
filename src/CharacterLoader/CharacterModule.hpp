//
// Created by PinkySmile on 08/10/24.
//

#ifndef CHARACTERENGINE_CHARACTERMODULE_HPP
#define CHARACTERENGINE_CHARACTERMODULE_HPP


#include <string>
#include <vector>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <SokuLib.hpp>
#include <filesystem>

class CharacterModule {
private:
	std::filesystem::path _folder;
	HMODULE _module = nullptr;
	SokuLib::v2::Player *(*_buildCharacter)(SokuLib::PlayerInfo &);
	size_t (*_getCharacterSize)();
	size_t (*_getObjectSize)();

	std::string _internalName;
	int _index;
	bool _indexNamedAssets;
	std::string _dll;
	std::string _data;
	std::string _skillSpriteSheet;
	std::string _shortName;
	std::string _fullName;
	std::vector<unsigned> _skillInputs;

public:
	CharacterModule(const nlohmann::json &json, const std::filesystem::path &folder);
	~CharacterModule();

	void load();
	void unload();
	void registerGR() const;
	SokuLib::v2::Player *build(SokuLib::PlayerInfo &p) const;

	[[nodiscard]] const std::string &getInternalName() const;
	[[nodiscard]] int getIndex() const;
	[[nodiscard]] bool isIndexNamedAssets() const;
	[[nodiscard]] const std::string &getDll() const;
	[[nodiscard]] const std::string &getData() const;
	[[nodiscard]] const std::string &getSkillSpriteSheet() const;
	[[nodiscard]] const std::string &getShortName() const;
	[[nodiscard]] const std::string &getFullName() const;
	[[nodiscard]] const std::vector<unsigned int> &getSkillInputs() const;
	[[nodiscard]] const std::filesystem::path &getFolder() const;
};


#endif //CHARACTERENGINE_CHARACTERMODULE_HPP
