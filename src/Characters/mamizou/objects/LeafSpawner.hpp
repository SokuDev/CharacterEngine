//
// Created by PinkySmile on 14/01/25.
//

#ifndef CHARACTERENGINE_LEAFSPAWNER_HPP
#define CHARACTERENGINE_LEAFSPAWNER_HPP


#include "BasicObject.hpp"

class LeafSpawner : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_LEAFSPAWNER_HPP
