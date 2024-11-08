//
// Created by PinkySmile on 02/11/24.
//

#ifndef CHARACTERENGINE_CIRNO_HPP
#define CHARACTERENGINE_CIRNO_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Cirno : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_CIRNO_HPP
