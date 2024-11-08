//
// Created by PinkySmile on 05/11/24.
//

#ifndef CHARACTERENGINE_SUWAKO_HPP
#define CHARACTERENGINE_SUWAKO_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Suwako : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_SUWAKO_HPP
