#pragma once
#ifdef __APPLE__
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#elif _WIN32
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#endif
#include "../Window.h"
#include "Camera/FPSCamera.h"

class World;

class Player
{

public:
	struct Config {
		bool showDebugChunks : 1;
		bool showDebug : 1;
		uint8_t MaxSelcedRange;
		int ViewDistanca;
		float mouseSensitivity;
		bool freeFly : 1;
	};

	Player(Window& window)
		:window(window),
		config{ false,false,100, 4, 0.001, false},
		camera(this, 90, 1600, 1200, 0.01)
	{
		window.player = this;
	}

	FPSCamera camera;

	glm::vec3 UpdatePlayer(float delta);

	bool updateView = false;

	inline const bool& GetMouseFree() const { return mouseFree; }
	inline const glm::vec3& GetPosition()	const { return position; }
	inline const Config& GetConfig()	const { return config; }

	void SetConfig(const Config& config) { this->config = config; }
	glm::vec3 position = { 0,0,0};

private:
	Config config;
	float playerspeed = 1;
	bool mouseFree = false;

	Window& window;
};

