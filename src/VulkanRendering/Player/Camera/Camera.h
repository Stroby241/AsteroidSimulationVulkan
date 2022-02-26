#pragma once
#include "glm/gtc/matrix_transform.hpp"

class Player;

class Camera
{

public:

	Camera(float fov, float width, float height);
	~Camera();
	virtual void update();
	virtual void updateProjection();

	glm::mat4 getViewPorj() { return viewPorj; }


	void SetFov(const float& fov) { this->fov = fov;  updateProjection();}
	void SetViewSize(const int& width, const int& height) { this->width = width; this->height = height; updateProjection(); }

	inline const float GetFov() const { return fov; }

protected:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewPorj;

	float fov;
	int width, height;
};

