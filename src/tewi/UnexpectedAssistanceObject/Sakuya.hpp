//
// Created by PinkySmile on 19/07/24.
//

#ifndef CHARACTERENGINE_SAKUYA_HPP
#define CHARACTERENGINE_SAKUYA_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Sakuya : public BasicObject {
	private:
		void _spawnActualBullet(unsigned id);
		bool _handleBorderBounce(float angle);

	public:
		void update() override;
		bool initializeAction() override;
	};
}


#endif //CHARACTERENGINE_SAKUYA_HPP
