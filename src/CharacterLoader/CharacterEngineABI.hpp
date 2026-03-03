//
// Created by PinkySmile on 02/03/2026.
//

#ifndef CHARACTERENGINE_CHARACTERENGINEABI_HPP
#define CHARACTERENGINE_CHARACTERENGINEABI_HPP


#include "CharacterEngine.hpp"
#include "CharacterModule.hpp"

SokuLib::v2::Player *createCharacter(int id, SokuLib::PlayerInfo &info);
extern CharacterEngineABI abi;
extern std::vector<CharacterModule> modules;
extern std::map<void *, std::shared_ptr<CharacterModule>> loadedModules;


#endif //CHARACTERENGINE_CHARACTERENGINEABI_HPP