#pragma once
#include "cocos2d.h"

enum class FrameStyle :unsigned short
{
	manual,
	idle,
	faceUP,
	faceDown,
	walkX,
	walkUP,
	walkDown,
	fire,
	fireUP,
	fireDown,
	jump,
	hurt,
	touched,
	win,
	die,
};
static const unsigned short FrameLength = static_cast<unsigned short>(FrameStyle::die) + 1;
struct FrameIndexes
{
	unsigned short index = 1;
	unsigned short start = 1;
	unsigned short end =1;
	FrameIndexes()
	{
		index = start;
	};
	FrameIndexes(const FrameIndexes& fi)
	{
		index = fi.index;
		start = fi.start;
		end = fi.end;
	};
	FrameIndexes(const unsigned short& i, const unsigned short&  s, const unsigned short&  e)
	{
		index = i;
		start = s;
		end = e;
	};
	FrameIndexes(const unsigned short&  s, const unsigned short&  e)
	{
		index = s;
		start = s;
		end = e;
	};
	inline bool isEnded() { return index == end; }
	void fetchNext()
	{
		if (index <= end)
			++index;
		else
			index = start;
	}

	cocos2d::SpriteFrame* getFrame(const char* name) 
	{
		char key[100];
		if (index > end)
			index = start;
		sprintf(key, "%s%d.png", name, index);
		return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(key);
	}
};
