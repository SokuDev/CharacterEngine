//
// Created by PinkySmile on 08/10/24.
//

#include "Memory.hpp"
#include "CharacterModule.hpp"
#include "giuroll.hpp"
#include "error.hpp"

CharacterModule::CharacterModule(const nlohmann::json &json, const std::filesystem::path &folder) :
	_folder(folder)
{
	if (!json.contains("internal_name"))
		throw std::runtime_error("internal_name missing");
	if (!json.contains("index"))
		throw std::runtime_error("index missing");
	if (!json.contains("index_named_assets"))
		throw std::runtime_error("index_named_assets missing");
	if (!json.contains("character_dll"))
		throw std::runtime_error("character_dll missing");
	if (!json.contains("data"))
		throw std::runtime_error("data missing");
	if (!json.contains("skill_sprite_sheet"))
		throw std::runtime_error("skill_sprite_sheet missing");
	if (!json.contains("short_name"))
		throw std::runtime_error("short_name missing");
	if (!json.contains("full_name"))
		throw std::runtime_error("full_name missing");
	if (!json.contains("skill_inputs"))
		throw std::runtime_error("skill_inputs missing");

	this->_internalName = json["internal_name"];
	this->_index = json["index"];
	this->_indexNamedAssets = json["index_named_assets"];
	this->_dll = json["character_dll"];
	this->_data = json["data"];
	this->_skillSpriteSheet = json["skill_sprite_sheet"];
	this->_shortName = json["short_name"];
	this->_fullName = json["full_name"];
	this->_skillInputs = json["skill_inputs"].get<std::vector<unsigned>>();
}

CharacterModule::~CharacterModule()
{
	if (!this->_module)
		return;
	FreeLibrary(this->_module);
}

void CharacterModule::load()
{
	auto path = this->_folder / this->_dll;

	this->_module = LoadLibraryW(path.wstring().c_str());
	if (!this->_module)
		throw std::runtime_error("Cannot load " + path.string() + ": LoadLibraryW: " + GetLastErrorAsString());

	try {
		this->_buildCharacter = reinterpret_cast<SokuLib::v2::Player *(*)(SokuLib::PlayerInfo &)>(GetProcAddress(this->_module, "buildCharacter"));
		if (!this->_buildCharacter)
			throw std::runtime_error("Cannot load " + path.string() + ": GetProcAddress(\"buildCharacter\"): " + GetLastErrorAsString());

		this->_getCharacterSize = reinterpret_cast<size_t (*)()>(GetProcAddress(this->_module, "getCharacterSize"));
		if (!this->_getCharacterSize)
			throw std::runtime_error("Cannot load " + path.string() + ": GetProcAddress(\"getCharacterSize\"): " + GetLastErrorAsString());

		this->_getObjectSize = reinterpret_cast<size_t (*)()>(GetProcAddress(this->_module, "getObjectSize"));
		if (!this->_getObjectSize)
			throw std::runtime_error("Cannot load " + path.string() + ": GetProcAddress(\"getObjectSize\"): " + GetLastErrorAsString());
	} catch (...) {
		FreeLibrary(this->_module);
		this->_module = nullptr;
		throw;
	}
}

void CharacterModule::unload()
{
	FreeLibrary(this->_module);
}

void CharacterModule::registerGR() const
{
	GiuRoll::setCharDataPos(this->_index, this->_getCharacterSize(), this->_getObjectSize());
}

SokuLib::v2::Player *CharacterModule::build(SokuLib::PlayerInfo &p) const
{
	return this->_buildCharacter(p);
}

const std::string &CharacterModule::getInternalName() const
{
	return this->_internalName;
}

int CharacterModule::getIndex() const
{
	return this->_index;
}

bool CharacterModule::isIndexNamedAssets() const
{
	return this->_indexNamedAssets;
}

const std::string &CharacterModule::getDll() const
{
	return this->_dll;
}

const std::string &CharacterModule::getData() const
{
	return this->_data;
}

const std::string &CharacterModule::getSkillSpriteSheet() const
{
	return this->_skillSpriteSheet;
}

const std::string &CharacterModule::getShortName() const
{
	return this->_shortName;
}

const std::string &CharacterModule::getFullName() const
{
	return this->_fullName;
}

const std::vector<unsigned int> &CharacterModule::getSkillInputs() const
{
	return this->_skillInputs;
}
