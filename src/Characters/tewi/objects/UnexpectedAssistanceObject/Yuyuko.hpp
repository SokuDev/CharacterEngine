//
// Created by PinkySmile on 15/10/24.
//

#ifndef CHARACTERENGINE_YUYUKO_HPP
#define CHARACTERENGINE_YUYUKO_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Yuyuko : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_YUYUKO_HPP
