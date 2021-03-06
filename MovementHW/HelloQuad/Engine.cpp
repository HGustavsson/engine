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

	objMan.createObj(Colliderless, keyring[0]);
	objMan.createObj(AABB, keyring[1], glm::vec3{ -.7, -.5, 0 }, glm::vec3{}, glm::vec3{ .3, .3, .3 });
	objMan.createObj(AABB, keyring[1], glm::vec3{ .7, -.5, 0 }, glm::vec3{}, glm::vec3{ .3, .3, .3 });
	objMan.createObj(Spherical, keyring[2], glm::vec3{ -.7, -.45, 0 }, glm::vec3{}, glm::vec3{ .1, .1, .1 });
	objMan.createObj(AABB, keyring[3], glm::vec3{ 0,-.5,0 }, glm::vec3{}, glm::vec3{ .1,.1,.1 });
	objMan.createObj(Spherical, keyring[4], glm::vec3{ .7, -.6,0 }, glm::vec3{}, glm::vec3{ .1, .1, .1 });
}

void Engine::updateObj(uint32_t num)
{
	//Physics?
	objMan.gameObjs[num].rigid.acceleration += ((objMan.gameObjs[num].rigid.force / objMan.gameObjs[num].rigid.mass)*deltaTime);
	objMan.gameObjs[num].rigid.velocity += objMan.gameObjs[num].rigid.acceleration;
	objMan.gameObjs[num].trans.locPos += (objMan.gameObjs[num].rigid.velocity * deltaTime);


	if (num == 3 && objMan.gameObjs[num].rigid.velocity.y > (float).0015)		//Limit
		objMan.gameObjs[num].rigid.velocity.y = (float).0015;
	else if (num == 3 && objMan.gameObjs[num].rigid.velocity.y < (float)-.002)	//Limit
		objMan.gameObjs[num].rigid.velocity.y = (float)-.002;
	if (num == 3 && objMan.gameObjs[num].rigid.velocity.x > (float).0015)		//Limit
		objMan.gameObjs[num].rigid.velocity.x = (float).0015;
	else if (num == 3 && objMan.gameObjs[num].rigid.velocity.x < (float)-.015)	//Limit
		objMan.gameObjs[num].rigid.velocity.x = (float)-.015;

	std::cout << objMan.gameObjs[3].trans.locPos.x << std::endl;
	std::cout << objMan.gameObjs[3].trans.locPos.y << std::endl;
	std::cout << objMan.gameObjs[3].trans.locPos.z << std::endl;
	objMan.gameObjs[num].rigid.force = { 0,0,0 }; //Zero it out so it doesn't keep applying.

	//Gen World Matrix
	glm::mat4 scaleMat = glm::scale(objMan.gameObjs[num].trans.locSize);
	glm::mat4 posMat = glm::translate(objMan.gameObjs[num].trans.locPos);
	glm::mat4 rotMat = glm::yawPitchRoll(objMan.gameObjs[num].trans.locRot.y, objMan.gameObjs[num].trans.locRot.x, objMan.gameObjs[num].trans.locRot.z);
	glm::mat4 worldMat = posMat*rotMat*scaleMat;
	objMan.gameObjs[num].trans.objWorldTransform = worldMat;
}

void Engine::addForce(glm::vec3 force, uint32_t numObj)
{
	objMan.gameObjs[numObj].rigid.force += force;
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
		for (int i = 0; i < objMan.gameObjs.size(); i++)
		{
			updateObj(i);
		}
		for (int i = 0; i < objMan.gameObjs.size(); i++)
		{
			glUniformMatrix4fv(2, 1, GL_FALSE, &objMan.gameObjs[i].trans.objWorldTransform[0][0]);
		
			//Draw buffered models
			glBindTexture(GL_TEXTURE_2D, texMap[objMan.gameObjs[i].imgName]);
			glBindVertexArray(vertArr);
			glDrawArrays(GL_TRIANGLES, 0, vertCount);
		}
		objMan.gameObjs[3].trans.locRot.z += .01;

		//Swap Buffers
		glfwSwapBuffers(glfwWindowPtr);

		//Process input/window events

		if (currInput[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(glfwWindowPtr, GL_TRUE);
		if (currInput[GLFW_KEY_A])
			addForce(glm::vec3{ -.0001, 0, 0 }, 3);
		if (currInput[GLFW_KEY_D])
			addForce(glm::vec3{ .0001, 0, 0 }, 3);		
		if (currInput[GLFW_KEY_W])
			addForce(glm::vec3{ 0, .0007, 0 }, 3);
		if (currInput[GLFW_KEY_S])
			addForce(glm::vec3{ 0, -.0001, 0 }, 3);
		
		//Gravity
		addForce(glm::vec3{ 0, -.0005, 0 }, 3); //Gravity
		//addForce(glm::vec3{gameObjs[3].rigid.velocity*-.3f}, 3); //Friction?

		//Keep it on the screen!
		if (objMan.gameObjs[3].trans.locPos.y < -0.9)
		{
			objMan.gameObjs[3].trans.locPos.y = -0.89;
		}
		
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
