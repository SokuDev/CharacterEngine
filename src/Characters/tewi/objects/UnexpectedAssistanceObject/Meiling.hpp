//
// Created by PinkySmile on 03/11/24.
//

#ifndef CHARACTERENGINE_MEILING_HPP
#define CHARACTERENGINE_MEILING_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Meiling : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_MEILING_HPP
