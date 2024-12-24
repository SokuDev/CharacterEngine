//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_GAMEOBJECTLIST_HPP
#define CHARACTERENGINE_GAMEOBJECTLIST_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

template<typename Factory>
class GameObjectList : public SokuLib::v2::IGameObjectList {
protected:
	void *_;
	SokuLib::Vector<std::unique_ptr<SokuLib::v2::GameObject>> _objects;
	SokuLib::Vector<unsigned int> _usedIndexes;
	SokuLib::List<unsigned int> _unusedIndexes;
	unsigned int _nextBase = 0;
	SokuLib::CriticalSection _mutex;
	SokuLib::List<SokuLib::v2::GameObject *> _list;
	SokuLib::v2::Player *_player;

public:
	inline GameObjectList(SokuLib::v2::Player* player) :
		_player(player)
	{
		static_assert(offsetof(SokuLib::v2::GameObjectList<SokuLib::v2::GameObject>, objectList) == offsetof(GameObjectList<Factory>, _list));
	}
	~GameObjectList() override = default;

	SokuLib::v2::GameObject *createObject(SokuLib::v2::GameObject* parentObj, SokuLib::v2::Player* owner, short action, float x, float y, char dir, char layer, void *data, unsigned int dataSize) override
	{
		SokuLib::v2::GameObject *obj = Factory::construct(this->_player, action);

		this->_objects.emplace_back(obj);
		this->_list.push_back(obj);
		obj->parentPlayer = this->_player;
		obj->parentPlayerB = this->_player;
		obj->gameData.owner = this->_player;
		obj->gameData.ally = owner;
		obj->gameData.opponent = owner->gameData.opponent;
		obj->textures = this->_player->textures;
		obj->gameData.patternMap = this->_player->gameData.patternMap;
		if (parentObj)
			((void (__thiscall *)(void *, void *))0x74C890)(obj, parentObj);
		obj->position.x = x;
		obj->position.y = y;
		obj->direction = dir;
		obj->layer = layer;
		if (data) {
			obj->customData = SokuLib::New<float>(dataSize);
			memcpy(obj->customData, data, dataSize * 4);
		}
		obj->setAction(action);
		return obj;
	}

	void clear() override
	{
		this->_list.clear();
		this->_objects.clear();
	}

	void updatePhysics() override
	{
		for (auto &o : this->_objects)
			o->updatePhysics();
	}

	void update() override
	{
		if (this->_player->gameData.opponent->timeStop == 0)
			for (size_t i = 0; i < this->_objects.size(); i++)
				this->_objects[i]->update();

		this->_list.erase(std::remove_if(this->_list.begin(), this->_list.end(), [](SokuLib::v2::GameObject *a){
			return a->lifetime == 0;
		}), this->_list.end());
		this->_objects.erase(std::remove_if(this->_objects.begin(), this->_objects.end(), [](std::unique_ptr<SokuLib::v2::GameObject> &a){
			return a->lifetime == 0;
		}), this->_objects.end());
	}

	void render1(char layer) override
	{
		for (auto &o : this->_objects) {
			if (o->layer != layer)
				continue;
			if (o->unknown154 != nullptr)
				((void (__thiscall *)(void *))0x4389D0)(o->unknown154);
			if (o->tail)
				o->tail->render();
			o->render();
			if (o->unknown154 != nullptr)
				((void (__thiscall *)(void *))0x438B90)(o->unknown154);
		}
	}

	void render2() override
	{
		for (auto &o : this->_objects)
			o->render2();
	}

	void updateCamera() override
	{
		for (auto &o : this->_objects)
			((void (*)(void *))0x46A780)(&*o);
	}

	void replaceOpponent(SokuLib::v2::Player *a0, SokuLib::v2::Player *a1) override
	{
		for (auto &o : this->_objects)
			if (o->gameData.opponent == a0)
				o->gameData.opponent = a1;
	}

	SokuLib::List<SokuLib::v2::GameObject *> &getList() override
	{
		return this->_list;
	}
};


#endif //CHARACTERENGINE_GAMEOBJECTLIST_HPP
