#include "Engine.h"


int main()
{
	Engine myEngine;

	if (!myEngine.init())
		return -1;

	if (!myEngine.bufferModel())
		return -1;

	if (!myEngine.useShaders())
		return -1;

	myEngine.addTex("textures/background.png");
	myEngine.addTex("textures/platform.png");
	myEngine.addTex("textures/sprite.png");
	myEngine.addTex("textures/block.png");
	myEngine.addTex("textures/teddy.png");
	
	myEngine.buffTex();

	myEngine.gameLoop();

	return 0;
}