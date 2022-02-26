#include "Camera.h"

Camera::Camera(float fov, float width, float height)
	: fov(fov), width(width), height(height)
{
	projection = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
	view = glm::mat4(1.0f);
	update();
}

Camera::~Camera() {}

void Camera::update()
{
	viewPorj = projection * view;
}

void Camera::updateProjection()
{
	projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
	update();
}



