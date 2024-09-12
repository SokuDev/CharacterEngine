//
// Created by PinkySmile on 04/09/24.
//

#ifndef CHARACTERENGINE_KILLINGRABBIT_HPP
#define CHARACTERENGINE_KILLINGRABBIT_HPP


#include "BasicObject.hpp"

class KillingRabbit : public BasicObject {
public:
	KillingRabbit() = default;
	~KillingRabbit() override = default;

	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_KILLINGRABBIT_HPP
