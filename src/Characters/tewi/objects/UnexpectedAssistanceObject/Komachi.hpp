//
// Created by PinkySmile on 27/10/24.
//

#ifndef CHARACTERENGINE_KOMACHI_HPP
#define CHARACTERENGINE_KOMACHI_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Komachi : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_KOMACHI_HPP
