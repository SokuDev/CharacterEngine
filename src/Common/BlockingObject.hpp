//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_BLOCKINGOBJECT_HPP
#define CHARACTERENGINE_BLOCKINGOBJECT_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class BlockingObject : public SokuLib::v2::GameObject {
public:
	BlockingObject() = default;
	~BlockingObject() override = default;

	void update() override;
	bool initializeAction() override;
	GameObject* createObject(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize) override;
	GameObject* createChild(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize) override;
};


#endif //CHARACTERENGINE_BLOCKINGOBJECT_HPP
