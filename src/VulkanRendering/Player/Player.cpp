#include "Player.h"

glm::vec3 Player::UpdatePlayer(float delta)
{

	bool* keys = window.keys;

	if (keys[GLFW_KEY_W]) {

		camera.moveFront(delta * playerspeed);
	}

	if (keys[GLFW_KEY_A]) {

		camera.moveSideways(delta * -playerspeed);

	}
	if (keys[GLFW_KEY_S]) {

		camera.moveFront(delta * -playerspeed);

	}
	if (keys[GLFW_KEY_D]) {

		camera.moveSideways(delta * playerspeed);


	}
	if (keys[GLFW_KEY_LEFT_SHIFT]) {

		camera.moveUp(delta * playerspeed);


	}
	if (keys[GLFW_KEY_SPACE]) {

		camera.moveUp(delta * -playerspeed);

	}

	if (keys[GLFW_KEY_LEFT_CONTROL]) {
		playerspeed = 10;
	}
	else {
		playerspeed = 1;
	}


	

	return { 0,0,0 };
}
