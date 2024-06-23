//
// Created by PinkySmile on 09/05/24.
//

#ifndef CHARACTERENGINE_BASICOBJECT_HPP
#define CHARACTERENGINE_BASICOBJECT_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class BasicObject : public SokuLib::v2::GameObject {
private:
#if defined(OBJECT_MEMORY_SIZE) && OBJECT_MEMORY_SIZE > 0
	char data[OBJECT_MEMORY_SIZE];
#endif

public:
	BasicObject() = default;
	~BasicObject() override = default;

	GameObject* createObject(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize) override;
	GameObject* createChild(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize) override;
};


#endif //CHARACTERENGINE_BASICOBJECT_HPP
