#include "App.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "Log.h"

#include <boost/polygon/voronoi.hpp>

App::App(){
    initImgui();

    VK_LOG_WARN("PushConstant Size {}", device.properties.limits.maxPushConstantsSize);
    globalPool = DescriptorPool::Builder(device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();

    LoadModel();

}

App::~App(){
    vkDestroyDescriptorPool(device.Getdevice(), imguiPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
}


void App::run() {

	std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);

	for (auto& i : uboBuffers) {
		i = std::make_unique<Buffer>(device, sizeof(GlobalUbo), 1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		i->map();
	}

	auto globalSetLayout = DescriptorSetLayout::Builder(device)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL)
		.addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_ALL)
		.build();


	std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < globalDescriptorSets.size(); i++)
	{
		auto bufferInfo1 = uboBuffers[i]->descriptorInfo();

		DesciptorWriter(*globalSetLayout, *globalPool)
			.writeBuffer(0, &bufferInfo1)
			.build(globalDescriptorSets[i]);
	}



	RenderSystem renderSystem{ device,window,renderer.getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout() };

	Player player{ window };
	player.camera.update();
	player.camera.SetFov(90);
	player.camera.updateProjection();

	auto currentTime = std::chrono::high_resolution_clock::now();


	std::vector<glm::mat4> transformData;

	for (int i = 0; i < 10; i++)
	{
		transformData.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, i, 0)));
	}

	while (!window.shouldClose()) {

		glfwPollEvents();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;


		ImGui::Begin("FPS");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		player.UpdatePlayer(frameTime);
		player.camera.update();

		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2{ float(window.width / 2 - 2),float(window.height / 2 - 2) }, ImVec2{ float(window.width / 2 + 2),float(window.height / 2 + 2) }, ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.5)));

		if (auto commandBuffer = renderer.beginFrame()) {

			int frameIndex = renderer.getFrameIndex();
			FrameInfo frameInfo{ frameIndex,frameTime,commandBuffer,player,globalDescriptorSets[frameIndex],0 };

			GlobalUbo ubo{};
			ubo.projectionView = player.camera.getViewPorj();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			renderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderGameObjects(frameInfo, models, transformData);

			ImGui::Render();
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}

	}

	vkDeviceWaitIdle(device.Getdevice());

}

static std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset, int id) {
	std::vector<uint32_t> indecies{
		1,0,2,3,0,1,
		4,5,6,4,7,5,
		8,9,10,8,11,9,
		13,12,14,15,12,13,
		16,17,18,16,19,17,
		21,20,22,23,20,21,
	};
	std::vector<Model::Vertex> vertices{

		// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f},id,1},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f},id,1},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f},id,1},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f},id,1},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f},id,2},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f},id,2},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f},id,2},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f},id,2},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f},id,3},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f},id,3},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f},id,3},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f},id,3},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f},id,4},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f},id,4},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f},id,4},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f},id,4},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f},id,5},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f},id,5},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f},id,5},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f},id,5},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f},id,6},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f},id,6},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f},id,6},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f},id,6},

	};
	for (auto& v : vertices) {
		v.position += offset;
	}
	return std::make_unique<Model>(device, vertices, indecies);
}

void App::LoadModel()
{
	auto temp = GameObject::createGameObject();
	std::shared_ptr<Model> tempModel = createCubeModel(device, { 0,0,0 }, temp.getId());
	temp.model = tempModel;
	temp.model->count = 10;
	temp.transform.translation = { 0,0,0 };
	temp.transform.scale = { 1.f,1.0f, 1.f };
	models.push_back(std::move(temp));
}


void App::initImgui() {

    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };


    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;


    vkCreateDescriptorPool(device.Getdevice(), &pool_info, nullptr, &imguiPool);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window.GetGLFWWindow(), true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.GetInstance();;
    init_info.PhysicalDevice = device.GetPhysicalDevice();
    init_info.Device = device.Getdevice();
    init_info.Queue = device.GetgraphicsQueue();
    init_info.DescriptorPool = imguiPool;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, renderer.getSwapChainRenderPass());

    auto commandBuffer = device.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    device.endSingleTimeCommands(commandBuffer);

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}
