//
// Created by PinkySmile on 12/10/24.
//

#ifndef CHARACTERENGINE_YOUMU_HPP
#define CHARACTERENGINE_YOUMU_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Youmu : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_YOUMU_HPP
