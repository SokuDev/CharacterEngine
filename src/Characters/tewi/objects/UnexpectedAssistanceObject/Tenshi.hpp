//
// Created by PinkySmile on 30/10/24.
//

#ifndef CHARACTERENGINE_TENSHI_HPP
#define CHARACTERENGINE_TENSHI_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Tenshi : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_TENSHI_HPP
