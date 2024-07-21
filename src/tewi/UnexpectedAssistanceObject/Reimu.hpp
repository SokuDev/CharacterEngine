//
// Created by PinkySmile on 17/07/24.
//

#ifndef CHARACTERENGINE_REIMU_HPP
#define CHARACTERENGINE_REIMU_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Reimu : public BasicObject {
	public:
		void update() override;
		bool initializeAction() override;
	};
}


#endif //CHARACTERENGINE_REIMU_HPP
