//
// Created by PinkySmile on 07/09/24.
//

#ifndef CHARACTERENGINE_RABBITSOLIDARITY_HPP
#define CHARACTERENGINE_RABBITSOLIDARITY_HPP


#include "BasicObject.hpp"

class RabbitSolidarity : public BasicObject {
private:
	void _hammerUpdate();
	void _hammerlessUpdate();

public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_RABBITSOLIDARITY_HPP
