//
// Created by PinkySmile on 25/03/2026.
//

#ifndef CHARACTERENGINE_YOUMUCLONE_HPP
#define CHARACTERENGINE_YOUMUCLONE_HPP


#include "BasicObject.hpp"

#define CLONE_BUFFERING_DELAY 12

class YoumuClone : public BasicObject {
private:
	struct State {
		SokuLib::SpriteEx sprite;
		SokuLib::v2::FrameData *img;
		SokuLib::v2::CharacterFrameData data;
		SokuLib::Vector2f pos;
		SokuLib::Vector2f center;
		SokuLib::RenderInfo infos;
		bool resetHit;
		char dir;
		unsigned char hitCount;
		bool transformed;
	};

	State _data[CLONE_BUFFERING_DELAY + 1];
	unsigned _dataPointer = 0;

public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_YOUMUCLONE_HPP