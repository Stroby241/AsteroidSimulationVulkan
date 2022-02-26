#include "Model.h"
#include <cassert>
Model::Model(Device& device, const std::vector<Vertex>& vertcies)
	: device(device)
{
	createVertexBuffer(vertcies);
}

Model::Model(Device& device, const std::vector<Vertex>& vertcies, const std::vector<uint32_t>& indecies)
	: device(device)
{
	createVertexBuffer(vertcies);
	createIndeciesBuffer(indecies);
}

Model::Model(Device& device)
	:device(device)
{
	

}

Model::~Model()
	
{
}

void Model::bind(VkCommandBuffer commandBuffer) const
{
	VkBuffer buffers[] = { vertexBuffer->getBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	if (hasIndexBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, indicesBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
}

void Model::Draw(VkCommandBuffer commandBuffer) const
{
	if (hasIndexBuffer) {
		vkCmdDrawIndexed(commandBuffer, indexCount, 10,0,0,0);
	}
	else {
		int firstElement = 0;
		if (count > 1) {
			firstElement = 1;
		}
		//vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		vkCmdDraw(commandBuffer, vertexCount, count, 0, firstElement);
	}
}

void Model::createVertexBuffer(const std::vector<Vertex>& vertcies)
{
	vertexCount = vertcies.size();
	assert(vertexCount >= 3 && "Vertex count must be at least 3");

	VkDeviceSize bufferSize = sizeof(vertcies[0]) * vertexCount;

	uint32_t vertexSize = sizeof(vertcies[0]);

	Buffer stagingbuffer(device, vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingbuffer.map();
	stagingbuffer.writeToBuffer((void*)vertcies.data());

	vertexBuffer = std::make_unique<Buffer>(device, vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	device.copyBuffer(stagingbuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);

}

void Model::createIndeciesBuffer(const std::vector<uint32_t>& indices)
{
	indexCount = indices.size();

	hasIndexBuffer = indexCount > 0;
	if (!hasIndexBuffer) {
		return;
	}

	VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

	uint32_t vertexSize = sizeof(indices[0]);

	Buffer stagingbuffer(device, vertexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingbuffer.map();
	stagingbuffer.writeToBuffer((void*)indices.data());

	indicesBuffer = std::make_unique<Buffer>(device, vertexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	device.copyBuffer(stagingbuffer.getBuffer(), indicesBuffer->getBuffer(), bufferSize);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(2);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	bindingDescriptions[1].binding = 1;
	bindingDescriptions[1].stride = sizeof(glm::mat4);
	bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
	return bindingDescriptions;
}
std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDiscriptions(8);
	attributeDiscriptions[0].binding = 0;
	attributeDiscriptions[0].location = 0;
	attributeDiscriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDiscriptions[0].offset = 0;

	attributeDiscriptions[1].binding = 0;
	attributeDiscriptions[1].location = 1;
	attributeDiscriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDiscriptions[1].offset = offsetof(Vertex,color);

	attributeDiscriptions[2].binding = 0;
	attributeDiscriptions[2].location = 2;
	attributeDiscriptions[2].format = VK_FORMAT_R32_SINT;
	attributeDiscriptions[2].offset = offsetof(Vertex, id);

	attributeDiscriptions[3].binding = 0;
	attributeDiscriptions[3].location = 3;
	attributeDiscriptions[3].format = VK_FORMAT_R32_SINT;
	attributeDiscriptions[3].offset = offsetof(Vertex, face);

	attributeDiscriptions[4].binding = 1;
	attributeDiscriptions[4].location = 4;
	attributeDiscriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDiscriptions[4].offset = 0;

	attributeDiscriptions[5].binding = 1;
	attributeDiscriptions[5].location = 5;
	attributeDiscriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDiscriptions[5].offset = sizeof(float) * 4;

	attributeDiscriptions[6].binding = 1;
	attributeDiscriptions[6].location = 6;
	attributeDiscriptions[6].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDiscriptions[6].offset = sizeof(float) * 8;

	attributeDiscriptions[7].binding = 1;
	attributeDiscriptions[7].location = 7;
	attributeDiscriptions[7].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDiscriptions[7].offset = sizeof(float) * 12;

	return attributeDiscriptions;
}
