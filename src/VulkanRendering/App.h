#pragma once
#include "VulkanRendering/Window.h"
#include "VulkanRendering/Device.h"
#include "VulkanRendering/Pipeline.h"
#include "VulkanRendering/SwapChain.h"
#include "VulkanRendering/Model.h"
#include "VulkanRendering/Renderer.h"
#include "VulkanRendering/render_system.h"
#include "VulkanRendering/Descriptor.h"

struct GlobalUbo
{
	glm::mat4 projectionView = glm::mat4{ 1.f };
	glm::vec2 cursorPos = { 0,0 };
	glm::vec2 windowSize = { 0,0 };
	glm::vec3 playerPos = { 0,0,0 };
};

struct Storage {
	int id = -1;
	int distance = 100;
	int face = 0;
};



class App {

public:
	static constexpr int WIDTH = 2560;
	static constexpr int HEIGHT = 1440;

	App();
	~App();

	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void run();

protected:
	void initImgui();
	Window window = { WIDTH, HEIGHT, "Hello Vulkan!" };
	Device device = { window };
	Renderer renderer = { window,device };

	std::unique_ptr<DescriptorPool> globalPool = {};
	VkDescriptorPool imguiPool;

	void LoadModel();
	std::vector<GameObject> models;
};
