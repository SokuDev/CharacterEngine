//
// Created by PinkySmile on 19/01/25.
//

#ifndef CHARACTERENGINE_BUCKETYOUKAI_HPP
#define CHARACTERENGINE_BUCKETYOUKAI_HPP


#include "BasicObject.hpp"

class BucketYoukai : public BasicObject {
public:
	void update() override;

	void initializeAction() override;
};


#endif //CHARACTERENGINE_BUCKETYOUKAI_HPP
