//
// Created by PinkySmile on 25/09/24.
//

#ifndef CHARACTERENGINE_ILLUSIONRABBIT_HPP
#define CHARACTERENGINE_ILLUSIONRABBIT_HPP


#include "BasicObject.hpp"

class IllusionRabbit : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_ILLUSIONRABBIT_HPP
