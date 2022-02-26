#pragma once
#include "Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include "Buffer.h"
#include <memory>

class Model {


public:

	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		int id;
		int face;
        
		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

	};

	Model(Device& device, const std::vector<Vertex>& vertcies);
	Model(Device& device, const std::vector<Vertex>& vertcies, const std::vector<uint32_t>& indecies);
	Model(Device& device);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandBuffer) const;
	void Draw(VkCommandBuffer commandBuffer) const;

	int count = 1;

private:
	void createVertexBuffer(const std::vector<Vertex>& vertcies);
	void createIndeciesBuffer(const std::vector<uint32_t>& indices);

	
	Device& device;
	
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indicesBuffer;

	uint32_t vertexCount;
	uint32_t indexCount;

	bool hasIndexBuffer = false;

};
