#include "Animation.h"
#include "../tinyxml/tinyxml.h"
#include <cstring>
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"

Animation::Animation()
{
	m_nCurFrame = 0;
}


Animation::~Animation()
{
	

	for (size_t i = 0; i < m_vframes.size(); i++)
	{
		delete m_vframes[i];
	}
	m_vframes.clear();
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void Animation::Next()
{
	if (m_nCurFrame+1 == m_vframes.size())
	{
		if (m_bLooping)
		{
			Reset();
			return;
		}
		else
		{
			//return to idle or play next animation
			if (m_sNextAnimation.length())
			{
				//play next animation
				PlayNext();
			}
			else
			{
				//play idle animation
				if (OwnerSystem)
					OwnerSystem->Play("Idle");
			}
		}
	}
	else
	{
		m_nCurFrame++;
		m_fTimeOnFrame = m_vframes[m_nCurFrame]->m_fdelay;
		if (m_vframes[m_nCurFrame]->m_sEvent != "NoEvent")
		{
			SendEvent(m_vframes[m_nCurFrame]->m_sEvent);
		}
		return;
	}
}
void Animation::PlayNext()
{
	if (OwnerSystem)
		OwnerSystem->Play(m_sNextAnimation);
}
void Animation::Render(float x, float y)
{


	SGD::Point drawpoint;
	drawpoint.x = x - m_vframes[m_nCurFrame]->m_szOffset.width;
	drawpoint.y = y - m_vframes[m_nCurFrame]->m_szOffset.height;
	
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, drawpoint, m_vframes[m_nCurFrame]->m_rDrawRect, 0, {}, {}, { 1.0f, 1.0f });
}
void Animation::Update(float dt)
{
	if (m_bPaused)
		return;

	m_fTimeOnFrame -= dt;
	if (m_fTimeOnFrame < 0.0f)
	{
		Next();
	}
}
void Animation::Load(std::string filename)
{
	//XML PARSING

	//HACK::TEST::TEST::REMOVE::ONCE::XML
	if (filename == "x" || filename == "z" || filename == "x2" || filename == "z2")
	{
		Frame *temp1 = new Frame;
		Frame *temp2 = new Frame;
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/rat.png");
		if (filename == "x")
		{
			temp1->m_fdelay = .5f;
			temp1->m_ptAnchorPoint = SGD::Point(148, 15);
			temp1->m_rDrawRect = SGD::Rectangle(137, 1, 158, 26);
			CalculateOffset(temp1->m_ptAnchorPoint, temp1->m_szOffset, temp1->m_rDrawRect);
			temp1->m_rCollisionRect = temp1->m_rDrawRect;
			temp1->m_sEvent = "TESTEVENT";

			temp2->m_fdelay = .5f;
			temp2->m_ptAnchorPoint = SGD::Point(169, 15);
			temp2->m_rDrawRect = SGD::Rectangle(159, 0, 178, 26);
			CalculateOffset(temp2->m_ptAnchorPoint, temp2->m_szOffset, temp2->m_rDrawRect);
			temp2->m_rCollisionRect = temp2->m_rDrawRect;
			temp2->m_sEvent = "NoEvent";

			m_sName = "UP";
			m_sNextAnimation = "Down";
			m_bLooping = false;
		}
		else if (filename == "z")
		{
			temp1->m_fdelay = .5f;
			temp1->m_ptAnchorPoint = SGD::Point(149, 38);
			temp1->m_rDrawRect = SGD::Rectangle(139, 27, 159, 52);
			CalculateOffset(temp1->m_ptAnchorPoint, temp1->m_szOffset, temp1->m_rDrawRect);
			temp1->m_rCollisionRect = temp1->m_rDrawRect;
			temp1->m_sEvent = "NoEvent";

			temp2->m_fdelay = .5f;
			temp2->m_ptAnchorPoint = SGD::Point(168, 37);
			temp2->m_rDrawRect = SGD::Rectangle(159, 27, 178, 52);
			CalculateOffset(temp2->m_ptAnchorPoint, temp2->m_szOffset, temp2->m_rDrawRect);
			temp2->m_rCollisionRect = temp2->m_rDrawRect;
			temp2->m_sEvent = "NoEvent";
			m_sName = "Down";
			m_bLooping = true;
		}
		else if (filename == "z2")
		{
			temp1->m_fdelay = 1.0f;
			temp1->m_ptAnchorPoint = SGD::Point(149, 38);
			temp1->m_rDrawRect = SGD::Rectangle(139, 27, 159, 52);
			CalculateOffset(temp1->m_ptAnchorPoint, temp1->m_szOffset, temp1->m_rDrawRect);
			temp1->m_rCollisionRect = temp1->m_rDrawRect;

			temp2->m_fdelay = 1.5f;
			temp2->m_ptAnchorPoint = SGD::Point(168, 37);
			temp2->m_rDrawRect = SGD::Rectangle(159, 27, 178, 52);
			CalculateOffset(temp2->m_ptAnchorPoint, temp2->m_szOffset, temp2->m_rDrawRect);
			temp2->m_rCollisionRect = temp2->m_rDrawRect;
			m_sName = "Down";
			m_bLooping = true;
		}
		else if (filename == "x2")
		{
			temp1->m_fdelay = 1.2f;
			temp1->m_ptAnchorPoint = SGD::Point(148, 15);
			temp1->m_rDrawRect = SGD::Rectangle(137, 1, 158, 26);
			CalculateOffset(temp1->m_ptAnchorPoint, temp1->m_szOffset, temp1->m_rDrawRect);
			temp1->m_rCollisionRect = temp1->m_rDrawRect;
			temp1->m_sEvent = "TESTEVENT";

			temp2->m_fdelay = 1.2f;
			temp2->m_ptAnchorPoint = SGD::Point(169, 15);
			temp2->m_rDrawRect = SGD::Rectangle(159, 0, 178, 26);
			CalculateOffset(temp2->m_ptAnchorPoint, temp2->m_szOffset, temp2->m_rDrawRect);
			temp2->m_rCollisionRect = temp2->m_rDrawRect;

			m_sName = "UP";
			m_sNextAnimation = "Down";
			m_bLooping = false;
		}
		m_vframes.push_back(temp1);
		m_vframes.push_back(temp2);
	}
	else
	{
		TiXmlDocument doc;
		if (doc.LoadFile(filename.c_str()) == false)
			return;
		//doc.Parse(filename.c_str());
		TiXmlElement* pRoot = doc.RootElement();

		if (pRoot == nullptr)
			return;


		int NumberFrames = 0;
		//TiXmlElement* imagepath = pRoot->FirstChildElement("Image")->FirstChildElement("Path");
		//const char* imagefilepath = imagepath->GetText();
		//NExtSiblingElement

		TiXmlElement* childs = pRoot->FirstChildElement("Image");

		childs->Attribute("numFrames", &NumberFrames);
		
		const char* imagefilepath = childs->FirstChildElement("Path")->GetText();
		
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(imagefilepath);
	
		const char* loopingstr = childs->NextSiblingElement("Looping")->GetText();

		if (strcmp(loopingstr, "False") == 0)
		{
			m_bLooping = false;
		}
		else
			m_bLooping = true;

		m_sName = childs->NextSiblingElement("Name")->GetText();
		m_sNextAnimation = childs->NextSiblingElement("NextName")->GetText();

		TiXmlElement* framess = childs->NextSiblingElement("Frame");
		for (int i = 0; i < NumberFrames; i++)
		{
			Frame* temp = new Frame;
			double dub;
			framess->Attribute("delay", &dub);
			temp->m_fdelay = (float)dub;
			TiXmlElement* AP = framess->FirstChildElement("AnchorPoint");
			AP->Attribute("x", &dub);
			temp->m_ptAnchorPoint.x = (float)dub;
			AP->Attribute("y", &dub);
			temp->m_ptAnchorPoint.y = (float)dub;


			TiXmlElement* DrawRect = framess->FirstChildElement("DrawRect");
			DrawRect->Attribute("left", &dub);
			temp->m_rDrawRect.left = float(dub);
			DrawRect->Attribute("top", &dub);
			temp->m_rDrawRect.top = float(dub);
			DrawRect->Attribute("right", &dub);
			temp->m_rDrawRect.right = float(dub);
			DrawRect->Attribute("bottom", &dub );
			temp->m_rDrawRect.bottom = float(dub);

			TiXmlElement* CollRect = framess->FirstChildElement("CollisionRect");
			DrawRect->Attribute("left", &dub);
			temp->m_rCollisionRect.left = (float)dub;
			DrawRect->Attribute("top", &dub );
			temp->m_rCollisionRect.top = (float)dub;
			DrawRect->Attribute("right", &dub );
			temp->m_rCollisionRect.right = (float)dub;
			DrawRect->Attribute("bottom", &dub);
			temp->m_rCollisionRect.bottom = (float)dub;

			temp->m_sEvent = framess->FirstChildElement("Event")->GetText();

			CalculateOffset(temp->m_ptAnchorPoint, temp->m_szOffset, temp->m_rDrawRect);


			m_vframes.push_back(temp);

			framess = framess->NextSiblingElement("Frame");

		}



	}

		m_fTimeOnFrame = m_vframes[0]->m_fdelay;
		m_nCurFrame = 0;
		m_bPaused = true;
	
}
void Animation::Reset()
{
	m_nCurFrame = 0;
	m_fTimeOnFrame = m_vframes[0]->m_fdelay;

	if (m_vframes[m_nCurFrame]->m_sEvent != "NoEvent")
	{
		SendEvent(m_vframes[m_nCurFrame]->m_sEvent);
	}
}
void Animation::Play()
{
	m_fTimeOnFrame = m_vframes[m_nCurFrame]->m_fdelay;
	m_bPaused = false;

	if (m_vframes[m_nCurFrame]->m_sEvent != "NoEvent")
	{
		SendEvent(m_vframes[m_nCurFrame]->m_sEvent);
	}

}
void Animation::Pause()
{
	m_bPaused = true;
}
bool Animation::isColliding(BaseObject* ob)
{
	if (OwnerSystem->m_position.x == -1000)
		return false;

	SGD::Rectangle rect1;
	SGD::Size coloffset;

	CalculateOffset(m_vframes[m_nCurFrame]->m_ptAnchorPoint, coloffset, m_vframes[m_nCurFrame]->m_rDrawRect);

	rect1.left = OwnerSystem->m_position.x - coloffset.width;
	//rect1.left = OwnerSystem->m_position.x + (m_vframes[m_nCurFrame]->m_ptAnchorPoint.x - m_vframes[m_nCurFrame]->m_rCollisionRect.left);
	//rect1.top = OwnerSystem->m_position.y + (m_vframes[m_nCurFrame]->m_ptAnchorPoint.y - m_vframes[m_nCurFrame]->m_rCollisionRect.top);
	rect1.top = OwnerSystem->m_position.y - coloffset.height;
	rect1.bottom = rect1.top + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeHeight();
	rect1.right = rect1.left + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeWidth();


	SGD::Rectangle rect2 = ob->GetRect();
	return rect1.IsIntersecting(rect2);
}
bool Animation::isColliding(Animation* a)
{
	if (OwnerSystem->m_position.x == -1000 || a->OwnerSystem->m_position.x == -1000)
		return false;
	SGD::Rectangle rect1;

	SGD::Size coloffset;

	CalculateOffset(m_vframes[m_nCurFrame]->m_ptAnchorPoint, coloffset, m_vframes[m_nCurFrame]->m_rDrawRect);


	//rect1.left =OwnerSystem->m_position.x + (m_vframes[m_nCurFrame]->m_ptAnchorPoint.x - m_vframes[m_nCurFrame]->m_rCollisionRect.left);
	//rect1.top = OwnerSystem->m_position.y + (m_vframes[m_nCurFrame]->m_ptAnchorPoint.y - m_vframes[m_nCurFrame]->m_rCollisionRect.top);
	rect1.left = OwnerSystem->m_position.x - coloffset.width;
	rect1.top = OwnerSystem->m_position.y - coloffset.height;
	rect1.bottom = rect1.top + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeHeight();
	rect1.right = rect1.left + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeWidth();


	SGD::Rectangle rect2;

	CalculateOffset(a->m_vframes[m_nCurFrame]->m_ptAnchorPoint, coloffset, a->m_vframes[m_nCurFrame]->m_rDrawRect);

	rect2.left = a ->OwnerSystem->m_position.x - coloffset.width;
	rect2.top = a->OwnerSystem->m_position.y - coloffset.height;
	rect2.bottom = rect2.top + a->m_vframes[a->m_nCurFrame]->m_rCollisionRect.ComputeHeight();
	rect2.right = rect2.left + a->m_vframes[a->m_nCurFrame]->m_rCollisionRect.ComputeWidth();

	return rect1.IsIntersecting(rect2);
}
void Animation::CalculateOffset(SGD::Point AP, SGD::Size& offset, SGD::Rectangle DrawRect)
{
	offset.width = AP.x - DrawRect.left;
	offset.height = AP.y - DrawRect.top;

}
void Animation::SendEvent(std::string eventname)
{
	if (eventname == "TESTEVENT")
	{
		SGD::Event* test = new SGD::Event("test");
		test->QueueEvent(nullptr);
		
	}
	else
	{
		SGD::Event* Trigger = new SGD::Event(eventname.c_str());
		Trigger->QueueEvent(nullptr);
	}
}
SGD::Rectangle Animation::GetColRect()
{
	SGD::Rectangle rect1;
	SGD::Size coloffset;
	CalculateOffset(m_vframes[m_nCurFrame]->m_ptAnchorPoint, coloffset, m_vframes[m_nCurFrame]->m_rCollisionRect);

	rect1.left = OwnerSystem->m_position.x - coloffset.width;
	rect1.top = OwnerSystem->m_position.y - coloffset.height;
	rect1.bottom = rect1.top + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeHeight();
	rect1.right = rect1.left + m_vframes[m_nCurFrame]->m_rCollisionRect.ComputeWidth();

	return rect1;
}