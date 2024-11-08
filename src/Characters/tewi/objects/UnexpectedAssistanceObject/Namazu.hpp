//
// Created by PinkySmile on 06/11/24.
//

#ifndef CHARACTERENGINE_NAMAZU_HPP
#define CHARACTERENGINE_NAMAZU_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Namazu : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_NAMAZU_HPP
