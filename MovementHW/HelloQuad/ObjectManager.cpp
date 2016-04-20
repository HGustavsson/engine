#include "ObjectManager.h"



ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::createObj(Collider cldr, const char* imgName)
{
	Object obj = 
	{ 
		{ glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 }, glm::mat4{} },
		{ 10, glm::vec3{}, glm::vec3{}, glm::vec3{} }, 
		cldr, 
		imgName 
	};
	gameObjs.push_back(obj);
}

void ObjectManager::createObj(Collider cldr, const char* imgName, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	Object obj = 
	{
		{ pos, rot, scale, glm::mat4{} },
		{ 10, glm::vec3{}, glm::vec3{}, glm::vec3{} },
		cldr,
		imgName 
	};
	gameObjs.push_back(obj);
}

void ObjectManager::createObj(Collider cldr, const char* imgName, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, float mass, glm::vec3 velocity)
{
	Object obj = 
	{ 
		{ pos, rot, scale, glm::mat4{} },
		{ mass, velocity, glm::vec3{}, glm::vec3{} },
		cldr,
		imgName 
	};
	gameObjs.push_back(obj);
}

bool ObjectManager::collides(Object objA, Object objB)
{
	float dist;
	if (objA.cldr.type == Colliderless || objB.cldr.type == Colliderless)
		return false;
	else if (objA.cldr.type == Spherical && objB.cldr.type == Spherical)
	{
		dist =
			glm::pow((objA.trans.locPos.x - objB.trans.locPos.x), 3) +
			glm::pow((objA.trans.locPos.y - objB.trans.locPos.y), 3) +
			glm::pow((objA.trans.locPos.z - objB.trans.locPos.z), 3);
		if (dist > glm::pow((objA.cldr.rad + objB.cldr.rad), 3))
		{
			
		}
		break;
	}
}
