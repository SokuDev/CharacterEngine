//
// Created by PinkySmile on 04/07/24.
//

#ifndef CHARACTERENGINE_SPINNINGSMOKE_HPP
#define CHARACTERENGINE_SPINNINGSMOKE_HPP


#include "BasicObject.hpp"

class SpinningSmoke : public BasicObject {
public:
	void update() override;

	bool initializeAction() override;
};


#endif //CHARACTERENGINE_SPINNINGSMOKE_HPP
