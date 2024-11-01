//
// Created by PinkySmile on 26/10/24.
//

#ifndef CHARACTERENGINE_REISEN_HPP
#define CHARACTERENGINE_REISEN_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Reisen : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_REISEN_HPP
