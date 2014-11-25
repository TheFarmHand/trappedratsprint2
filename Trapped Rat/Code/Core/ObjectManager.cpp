
#include "ObjectManager.h"
#include "BaseObject.h"



void ObjectManager::UpdateAll(float dt)
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->Update(dt);
	}
}
void ObjectManager::RenderAll()
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->Render();
	}
}

bool  ObjectManager::Find(BaseObject * obj)
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i] == obj)
		{
			return true;
		}
	}
	return false;
}
void  ObjectManager::Remove(BaseObject * obj)
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i] == obj)
			Objects.erase(Objects.begin() + i);
	}
}
void  ObjectManager::Add(BaseObject * obj)
{
	Objects.push_back(obj);
}


void ObjectManager::Clean()
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i]!= nullptr)
			delete Objects[i];
		Objects[i] = nullptr;
		Objects.erase(Objects.begin() + i);
	}
}

void ObjectManager::Collision()
{
	for (unsigned int i = 0; i < Objects.size(); i++)
	{
		for (unsigned int j = 0; j < Objects.size(); j++)
		{
			if (Objects[i]->GetRect().IsIntersecting(Objects[j]->GetRect()))
			{
				Objects[i]->HandleCollision(Objects[j]);
			}
		}
	}
}
