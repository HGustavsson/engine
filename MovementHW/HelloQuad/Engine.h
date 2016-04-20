#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <FreeImage.h>
#include <string>
#include "ShaderManager.h"
#include "ObjectManager.h"
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
	ObjectManager objMan;
	std::map<const char*, GLuint> texMap;
	std::vector<const char*> keyring;
	float prevFrame;
	float deltaTime;
	float currFrame;

	//Private Methods
	void addForce(glm::vec3, uint32_t);
	void updateObj(uint32_t num);
};