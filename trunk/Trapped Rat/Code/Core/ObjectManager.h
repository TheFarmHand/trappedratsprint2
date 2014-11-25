#pragma once
#include <vector>

class BaseObject;
class ObjectManager
{
private:
	std::vector<BaseObject*> Objects;
		
public:
	
	ObjectManager() = default;
	~ObjectManager() = default;


	void UpdateAll(float dt);//Update all objects in vector
	void RenderAll();//render all objects in vector
	bool Find(BaseObject * obj);//confirms if a pointer is the vector
	void Remove(BaseObject * obj);//removes pointer, does not delete it
	void Add(BaseObject * obj);//pushes a point onto the vector
	void Clean();//clear all objects in vector and deallocates memory
	void Collision();

};

