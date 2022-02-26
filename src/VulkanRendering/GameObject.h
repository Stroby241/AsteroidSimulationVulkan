#pragma once

#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>


struct RigidBody {
	glm::vec3 velocity;
	float mass = 1;
};


struct TransformComponent {
	glm::vec3 translation = {};  // (position offset)
	glm::vec3 scale = { 1.f, 1.f,1.f };
	glm::vec3 rotation = { 0,0,0 };

	glm::mat4 mat4() {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},
			{translation.x, translation.y, translation.z, 1.0f} };
	}

};


class GameObject {

public:
	static GameObject createGameObject() {
		static uint32_t currentId = 0;
		return GameObject{ currentId++ };
	}

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	uint32_t getId() { return id; }

	std::shared_ptr<Model> model;
	glm::vec3 color = { 1,1,1 };
	TransformComponent transform;
	RigidBody rigidBody;

private:
	GameObject(uint32_t objId) : id{ objId } {}

	uint32_t id;
};
