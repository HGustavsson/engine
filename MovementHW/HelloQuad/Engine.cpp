#include "Engine.h"

namespace		//Use minimally!
{
	std::map<int, bool> currInput;	//the int is supposed to be a GLFW #define such as GLFW_KEY_ESCAPE
	std::map<int, bool> prevInput;	

	//If someone clicks...
	void mouseClick(GLFWwindow* glfwWindowPtr, int button, int action, int mods)
	{
		currInput[button] = action;
	}

	//If someone presses a key...
	void keyCallback(GLFWwindow* glfwWindowPtr, int key, int scancode, int action, int mods)
	{
		currInput[key] = action;
	}
}

Engine::Engine()
{
}

Engine::~Engine()
{
	for each (std::pair<const char*, GLuint> keyValPair in texMap)
	{
		glDeleteTextures(1, &keyValPair.second);
	}
}

bool Engine::init()
{
	//Initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		return false;
	}
	glfwWindowPtr = glfwCreateWindow(800, 800, "Hawkon Gustavsson DSA1 Engine", NULL, NULL);
	if (glfwWindowPtr != nullptr)
		glfwMakeContextCurrent(glfwWindowPtr);
	else
	{
		glfwTerminate();
		return false;
	}
	//Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}
	return true;
}

bool Engine::bufferModel()
{
	//Create Quad**
	std::vector<Vertex> verts =
	{ { { 1.0, 1.0, 0 }, { 1.0, 1.0 } }, 
	  { { -1.0, 1.0, 0 }, { 0.0, 1.0 } }, 
	  { { -1.0, -1.0, 0 }, { 0.0, 0.0 } }, 
	  { { 1.0, -1.0, 0 }, { 1.0, 0.0 } } };

	std::vector <unsigned int> locInds = { 0, 1, 2, 0, 2, 3 };
	vertCount = locInds.size();

	//Duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);
	for (uint32_t i = 0; i < vertCount; i++)
	{
		vertBufData[i].loc = verts[locInds[i]].loc;
		vertBufData[i].uv = verts[locInds[i]].uv;
	}

	GLuint vertBuf;
	//Generate Buffer
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	//Bind Buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	//Store data in the Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &vertBufData[0], GL_STATIC_DRAW);

	//Describe Buffer Contents
	glEnableVertexAttribArray(0);
	//Location AttribPtr
	glVertexAttribPointer(
		0,					//Attribute index
		3,					//Number of components
		GL_FLOAT,			//Data Type
		GL_FALSE,			//Does it need to be normalized?
		sizeof(Vertex),		//Bytes per vertex
		0);					//Offset
	//UV AttribPtr
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							//Attribute index
		2,							//Number of components
		GL_FLOAT,					//Data Type
		GL_FALSE,					//Does it need to be normalized?
		sizeof(Vertex),				//Bytes per vertex
		(void*)sizeof(glm::vec3));	//Offset

	//Set the ClearColor
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

	//To be adjusted to account for failed loading
	//always true for now
	return true;
}

void Engine::addTex(const char* texName)
{
	//Load Image
	const char* imgName = texName; //Why can't I delete this?!?!?!?!
	keyring.push_back(imgName);

	GLuint texID;
	glGenTextures(1, &texID);

	texMap[imgName] = texID; 
}

void Engine::buffTex()
{
	for each (std::pair<const char*, GLuint> keyValPair in texMap)
	{
		glBindTexture(GL_TEXTURE_2D, keyValPair.second);
		FIBITMAP* imgPtr = FreeImage_Load(FreeImage_GetFileType(keyValPair.first, 0), keyValPair.first);
		FIBITMAP* imgPtr32 = FreeImage_ConvertTo32Bits(imgPtr);

		//Upload texture bytes
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(imgPtr32), FreeImage_GetHeight(imgPtr32),
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(imgPtr32));

		//Set min filter to linear instead of mipmap linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Clear it from RAM
		FreeImage_Unload(imgPtr);
		FreeImage_Unload(imgPtr32);

		//Unbind it
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	createObj(keyring[0]);
	createObj(keyring[1], glm::vec3{ -.7, -.5, 0 }, glm::vec3{}, glm::vec3{ .3, .3, .3 });
	createObj(keyring[1], glm::vec3{ .7, -.5, 0 }, glm::vec3{}, glm::vec3{ .3, .3, .3 });
	createObj(keyring[2], glm::vec3{ -.7, -.45, 0 }, glm::vec3{}, glm::vec3{ .1, .1, .1 });
	createObj(keyring[3], glm::vec3{ 0,-.5,0 }, glm::vec3{}, glm::vec3{ .1,.1,.1 });
	createObj(keyring[4], glm::vec3{ .7, -.6,0 }, glm::vec3{}, glm::vec3{ .1, .1, .1 });
}

void Engine::createObj(const char* imgName)
{
	Object obj = { {glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1},
		glm::mat4{} },{10, glm::vec3{}, glm::vec3{}, glm::vec3{}}, imgName };
	gameObjs.push_back(obj);
}

void Engine::createObj(const char* imgName, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	Object obj = { { pos, rot, scale, glm::mat4{} }, { 10, glm::vec3{}, glm::vec3{}, glm::vec3{} }, imgName };
	gameObjs.push_back(obj);
}

void Engine::createObj(const char* imgName, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, float mass, glm::vec3 velocity)
{
	Object obj = { { pos, rot, scale, glm::mat4{} },{ mass, velocity, glm::vec3{}, glm::vec3{} }, imgName };
	gameObjs.push_back(obj);
}

void Engine::updateObj(uint32_t num)
{
	//Physics?
	gameObjs[num].rigid.acceleration = ((gameObjs[num].rigid.force / gameObjs[num].rigid.mass)*deltaTime);
	gameObjs[num].rigid.velocity += gameObjs[num].rigid.acceleration;
	gameObjs[num].trans.locPos += gameObjs[num].rigid.velocity;

	gameObjs[num].rigid.force = { 0,0,0 }; //Zero it out so it doesn't keep applying.

	//Gen World Matrix
	glm::mat4 scaleMat = glm::scale(gameObjs[num].trans.locSize);
	glm::mat4 posMat = glm::translate(gameObjs[num].trans.locPos);
	glm::mat4 rotMat = glm::yawPitchRoll(gameObjs[num].trans.locRot.y, gameObjs[num].trans.locRot.x, gameObjs[num].trans.locRot.z);
	glm::mat4 worldMat = posMat*rotMat*scaleMat;
	gameObjs[num].trans.objWorldTransform = worldMat;
}

bool Engine::gameLoop()
{
	glfwSetMouseButtonCallback(glfwWindowPtr, mouseClick);
	glfwSetKeyCallback(glfwWindowPtr, keyCallback);

	//Transparency! **Warning: May break later!**
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Game Loop
	while (!glfwWindowShouldClose(glfwWindowPtr))
	{
		//Time
		if(currFrame != 0)
			deltaTime = currFrame - prevFrame;
		currFrame = glfwGetTime();

		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		//Update physical sim
		for (int i = 0; i < gameObjs.size(); i++)
		{
			updateObj(i);
		}
		for (int i = 0; i < gameObjs.size(); i++)
		{
			glUniformMatrix4fv(2, 1, GL_FALSE, &gameObjs[i].trans.objWorldTransform[0][0]);
		
			//Draw buffered models
			glBindTexture(GL_TEXTURE_2D, texMap[gameObjs[i].imgName]);
			glBindVertexArray(vertArr);
			glDrawArrays(GL_TRIANGLES, 0, vertCount);
		}

		//Swap Buffers
		glfwSwapBuffers(glfwWindowPtr);

		//Process input/window events

		if (currInput[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(glfwWindowPtr, GL_TRUE);

		//Process queued window, mouse & keyboard callback events
		prevInput = currInput;
		glfwPollEvents();
	}

	//End GLFW
	glfwTerminate();

	//Return
	return true;
}

bool Engine::useShaders()
{
	if (!shadMan.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl")) 
		return false;
	glUseProgram(shadMan.getProgram());
	return true;
}
