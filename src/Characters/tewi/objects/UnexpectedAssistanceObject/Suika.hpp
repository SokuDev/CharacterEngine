//
// Created by PinkySmile on 26/10/24.
//

#ifndef CHARACTERENGINE_SUIKA_HPP
#define CHARACTERENGINE_SUIKA_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Suika : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_SUIKA_HPP
