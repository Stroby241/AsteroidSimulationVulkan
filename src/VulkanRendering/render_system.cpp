#include "render_system.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "Log.h"


RenderSystem::RenderSystem(Device& device, Window& window, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	: device(device), window(window)
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
	instanceBuffer = std::make_unique<Buffer>(device, sizeof(glm::mat4), 300*300, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	instanceBuffer->map();
	VK_LOG_INFO("Test size: {}", sizeof(SimplePushConstantData));
}

RenderSystem::~RenderSystem()
{
	vkDestroyPipelineLayout(device.Getdevice(), pipelineLayout, nullptr);
}

void RenderSystem::renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects, const std::vector<glm::mat4>& data)
{
	pipeline->bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);


	for (auto& obj : gameObjects) {
		//obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + frameInfo.frameTime * 2, glm::two_pi<float>());
		//obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + frameInfo.frameTime * 2, glm::two_pi<float>());
		//obj.transform.rotation.z = glm::mod(obj.transform.rotation.z + frameInfo.frameTime * 2, glm::two_pi<float>());

		SimplePushConstantData push{};
		push.transform = obj.transform.mat4();
		push.color = obj.color;
		
		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

		if (obj.model->count == 1) {
			glm::mat4 temp(1.f);
			instanceBuffer->writeToBuffer(&temp, sizeof(glm::mat4));
		}
		else {
			instanceBuffer->writeToBuffer((void*)data.data(), sizeof(glm::mat4) * data.size());
		}

		VkBuffer insatnceBuffers[] = { instanceBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(frameInfo.commandBuffer, 1, 1, insatnceBuffers, offsets);
		obj.model->bind(frameInfo.commandBuffer);
		obj.model->Draw(frameInfo.commandBuffer);
	}
}


void RenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector< VkDescriptorSetLayout> descriptorSetLayout{ globalSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayout.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device.Getdevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void RenderSystem::createPipeline(VkRenderPass renderPass)
{
	auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(window.width, window.height);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(device, "Shader/vert.spv", "Shader/frag.spv", pipelineConfig);
}
