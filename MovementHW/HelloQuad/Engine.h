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

struct Vertex
{
	glm::vec3 loc;
	glm::vec2 uv;
};

/*Object Management*/

struct Transform
{
	glm::vec3 locPos;
	glm::vec3 locRot;
	glm::vec3 locSize;
	glm::mat4 objWorldTransform;
};

struct rigidBody
{
	float mass;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;
};

struct Object
{
	Transform trans;
	rigidBody rigid;
	const char* imgName;
};

/*Class Methods & Variables*/

class Engine
{
public:
	Engine();
	~Engine();
	bool init();
	bool bufferModel();
	bool gameLoop();
	bool useShaders();
	void addTex(const char* texName);
	void buffTex();

private:
	//Private variables & collections
	GLFWwindow* glfwWindowPtr;
	GLuint vertArr;
	uint32_t vertCount;
	ShaderManager shadMan;
	std::map<const char*, GLuint> texMap;
	std::vector<const char*> keyring;
	std::vector<Object> gameObjs;
	float prevFrame;
	float deltaTime;
	float currFrame;

	//Private Methods
	void createObj(const char*);
	void createObj(const char*, glm::vec3, glm::vec3, glm::vec3);
	void createObj(const char*, glm::vec3, glm::vec3, glm::vec3, float, glm::vec3);
	void addForce(glm::vec3);
	void updateObj(uint32_t num);
};