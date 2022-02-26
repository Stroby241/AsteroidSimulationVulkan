#pragma once
#include"Camera.h"

class Player;

class FPSCamera : public Camera
{

public:
	FPSCamera(Player* player, float fov, float width, float height, float mouseSpeed);

	void onMouseMoved(float xRel, float yRel);
	void update() override;
	void updateProjection() override;

	void moveFront(float amount);
	void moveSideways(float amount);
	void moveUp(float amount);

	void translate(glm::vec3 v);

	inline const glm::vec3& GetLookAt() const { return lookAt; }

	float yaw;
	float pitch;
	glm::vec3 lookAt = { 0,0,0 };
	glm::vec3 up = { 0,1,0 };
	const float mouseSensitivity = 0.01;
private:
	Player* player;
	glm::vec3 offset = { 0,0.3f,0 };
};

