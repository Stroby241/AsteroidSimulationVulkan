#include "FPSCamera.h"
#include "../Player.h"

FPSCamera::FPSCamera(Player* player,float fov, float width, float height,float mouseSpeed) 
	: Camera(fov, width, height), mouseSensitivity(mouseSpeed), player(player)
{
	yaw = 270.0f;
	pitch = 0.0f;
	onMouseMoved(0.0f, 0.0f);
	update();
}

void FPSCamera::onMouseMoved(float xRel, float yRel)
{
	yaw -= xRel * mouseSensitivity;
	pitch += yRel * mouseSensitivity;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = -sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	lookAt = glm::normalize(front);
	update();
}

void FPSCamera::update() {
	glm::vec3 pos = player->GetPosition();
	view = glm::lookAt(pos+offset, pos + lookAt, up);
	viewPorj = projection * view;
}

void FPSCamera::updateProjection()
{
	projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
	update();
}

void FPSCamera::moveFront(float amount)
{
	translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount);
	update();
}

void FPSCamera::moveSideways(float amount)
{
	translate(glm::normalize(glm::cross(lookAt, up)) * amount);
	update();
}

void FPSCamera::moveUp(float amount)
{
	translate(up * amount);
	update();
}

void FPSCamera::translate(glm::vec3 v)
{
	player->position += v;
	view = glm::translate(view, v + offset * -1.0f);
}
