#include "VulkanRendering/Device.h"
#include "VulkanRendering/Pipeline.h"
#include "VulkanRendering/SwapChain.h"
#include "VulkanRendering/GameObject.h"
#include "VulkanRendering/Buffer.h"
#include "Player/Player.h"


struct SimplePushConstantData {
	glm::mat4 transform{ 1.f };
	glm::vec3 color;
};

struct FrameInfo {
	int frameIndex;
	float frameTime;
	VkCommandBuffer commandBuffer;
	Player& player;
	VkDescriptorSet globalDescriptorSet;
	int selected;
};

struct InstacneData {
	glm::vec3 mat1;
	glm::vec3 mat2;
	glm::vec3 mat3;
	glm::vec3 mat4;
};

class RenderSystem{
public:

	RenderSystem(Device& device,Window& window,VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout = VK_NULL_HANDLE);
	~RenderSystem();

	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	void renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects, const std::vector<glm::mat4>& data);

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);

	Device& device;
	Window& window;
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
	std::unique_ptr<Buffer> instanceBuffer;
	

};
