#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include <string>
#include <vector>
class Frame
{
public:
	SGD::Rectangle m_rDrawRect;
	SGD::Rectangle m_rCollisionRect;
	std::string m_sEvent;
	SGD::Point m_ptAnchorPoint;
	SGD::Size m_szOffset;
	float m_fdelay;

	Frame();
	~Frame();
};

