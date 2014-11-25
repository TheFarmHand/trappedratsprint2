#include "AnimationSystem.h"
#include "../tinyxml/tinyxml.h"
#include "../Core/GameData.h"

AnimationSystem::AnimationSystem()
{
	m_position = SGD::Point(-1000, -1);
}


AnimationSystem::~AnimationSystem()
{
	UnloadAll();
}


void AnimationSystem::Render(float PositionX, float PositionnY)
{
	m_position.x = PositionX;
	m_position.y = PositionnY;


	SGD::Point camera = GameData::GetInstance()->GetCamera();
	SGD::Point center = { PositionX - camera.x, PositionnY - camera.y };


	if (m_nCurrentAnimation == -1)
		return;
	m_vAnimations[m_nCurrentAnimation]->Render(center.x, center.y);
}
void AnimationSystem::Update(float dt)
{
	if (m_nCurrentAnimation == -1)
		return;

	m_vAnimations[m_nCurrentAnimation]->Update(dt);
}

void AnimationSystem::Load(Animation* Anim)
{
	m_vAnimations.push_back(Anim);
	Anim->OwnerSystem = this;
}
void AnimationSystem::Load(std::string filename)
{
	filename = "../Trapped Rat/Assets/Scripts/" + filename;
	TiXmlDocument doc;
	if (doc.LoadFile(filename.c_str()) == false)
		return;
	//doc.Parse(filename.c_str());
	TiXmlElement* pRoot = doc.RootElement();

	if (pRoot == nullptr)
		return;
	int numAnimations;
	pRoot->Attribute("numAnimations", &numAnimations);
	pRoot = pRoot->FirstChildElement("Animation");
	
	for (int i = 0; i < numAnimations; i++)
	{
		Animation* Anim = new Animation;
		
		int NumberFrames = 0;
		//TiXmlElement* imagepath = pRoot->FirstChildElement("Image")->FirstChildElement("Path");
		//const char* imagefilepath = imagepath->GetText();
		//NExtSiblingElement

		TiXmlElement* childs = pRoot->FirstChildElement("Image");

		childs->Attribute("numFrames", &NumberFrames);

		std::string imagefilepath = childs->FirstChildElement("Path")->GetText();
		std::string imagepath = "../Trapped Rat/Assets/Textures/" + imagefilepath;

		Anim->m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(imagepath.c_str());

		const char* loopingstr = childs->NextSiblingElement("Looping")->GetText();

		if (strcmp(loopingstr, "False") == 0)
		{
			Anim->m_bLooping = false;
		}
		else
			Anim->m_bLooping = true;

		Anim->m_sName = childs->NextSiblingElement("Name")->GetText();
		Anim->m_sNextAnimation = childs->NextSiblingElement("NextName")->GetText();

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
			DrawRect->Attribute("bottom", &dub);
			temp->m_rDrawRect.bottom = float(dub);

			TiXmlElement* CollRect = framess->FirstChildElement("CollisionRect");
			DrawRect->Attribute("left", &dub);
			temp->m_rCollisionRect.left = (float)dub;
			DrawRect->Attribute("top", &dub);
			temp->m_rCollisionRect.top = (float)dub;
			DrawRect->Attribute("right", &dub);
			temp->m_rCollisionRect.right = (float)dub;
			DrawRect->Attribute("bottom", &dub);
			temp->m_rCollisionRect.bottom = (float)dub;

			temp->m_sEvent = framess->FirstChildElement("Event")->GetText();

			Anim->CalculateOffset(temp->m_ptAnchorPoint, temp->m_szOffset, temp->m_rDrawRect);


			Anim->m_vframes.push_back(temp);

			framess = framess->NextSiblingElement("Frame");
		}

		//PushBackAnimation
		//SetAnimationOwner
		Anim->OwnerSystem = this;
		m_vAnimations.push_back(Anim);
		
		pRoot = pRoot->NextSiblingElement("Animation");
		
	}
}
void AnimationSystem::ResetAll()
{
	for (size_t i = 0; i < m_vAnimations.size(); i++)
	{
		m_vAnimations[i]->Reset();
	}
}
void AnimationSystem::Play(std::string name)
{
	for (size_t i = 0; i < m_vAnimations.size(); i++)
	{
		if (m_vAnimations[i]->m_sName == name)
		{
			Play(i);
			return;
		}
	}
}
void AnimationSystem::Pause(bool is_pause)
{
	if (m_nCurrentAnimation == -1)
		return;
	if (is_pause)
	{
		m_vAnimations[m_nCurrentAnimation]->Pause();
	}
	else
	{
		m_vAnimations[m_nCurrentAnimation]->Play();
	}
}

void AnimationSystem::Play(int position)
{
	
	m_vAnimations[position]->Play();
	m_nCurrentAnimation = position;
}
void AnimationSystem::UnloadAll()
{
	for (size_t i = 0; i < m_vAnimations.size(); i++)
	{
		delete m_vAnimations[i];
	}
	m_vAnimations.clear();
}
bool AnimationSystem::CheckCollision(AnimationSystem* as)
{
	return m_vAnimations[m_nCurrentAnimation]->isColliding(as->m_vAnimations[m_nCurrentAnimation]);
}
bool AnimationSystem::CheckCollision(BaseObject* ob)
{
	return m_vAnimations[m_nCurrentAnimation]->isColliding(ob);
}
SGD::Rectangle AnimationSystem::GetColRect()
{
	return m_vAnimations[m_nCurrentAnimation]->GetColRect();
}

