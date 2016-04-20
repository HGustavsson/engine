#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <FreeImage.h>
#include <string>
#include "ShaderManager.h"
#include <vector>
#include <map>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
//Collider
enum Type
{
	AABB, Spherical, Colliderless
};

class ObjectManager
{
	struct Transform
	{
		glm::vec3 locPos;
		glm::vec3 locRot;
		glm::vec3 locSize;
		glm::mat4 objWorldTransform;
	};

	struct RigidBody
	{
		float mass;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 force;
	};

	//Collider Cont'd
	union Mesh
	{
		struct SphericalCldr
		{
			float rad;
		};
		struct AABBCldr
		{
			float minX;
			float maxX;
			float minY;
			float maxY;
			float minZ;
			float maxZ;
		};
	};

	struct Collider
	{
		Type type;
		Mesh mesh;
	};
	//Collider End

	struct Object
	{
		Transform trans;
		RigidBody rigid;
		Collider cldr;
		const char* imgName;
	};

public:
	ObjectManager();
	~ObjectManager();
	std::vector<Object> gameObjs;

	void createObj(Collider, const char*);
	void createObj(Collider, const char*, glm::vec3, glm::vec3, glm::vec3);
	void createObj(Collider, const char*, glm::vec3, glm::vec3, glm::vec3, float, glm::vec3);
	bool collides(Object, Object);
};

