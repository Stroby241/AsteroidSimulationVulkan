#pragma once 
#include "Device.h"

class SwapChain {

public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;


	SwapChain(Device& device,Window& window, VkExtent2D windowExtend);
	SwapChain(Device& device, Window& window, VkExtent2D windowExtend,VkSwapchainKHR swapChain);
	~SwapChain();

	SwapChain(const SwapChain&) = delete;
	void operator=(const SwapChain&) = delete;

	VkFramebuffer GetFrameBuffer(int index) const { return swapChainFramebuffers[index]; }
	VkRenderPass GetRenderPass() const { return renderPass; }
	size_t ImageCount() const { return swapChainImages.size(); }
	VkFormat GetSwapChainImageFormat() const { return swapChainImageFormat;  }
	VkExtent2D GetSwapChainExtent() const { return swapChainExtent; }
	uint32_t GetWidth() const { return swapChainExtent.width; }
	uint32_t GetHeight() const { return swapChainExtent.height; }

	VkFormat findDepthFormat();

	VkResult acquireNextImage(uint32_t* imageIndex);
	VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
private:
	void createSwapChain(VkSwapchainKHR OldswapChian = VK_NULL_HANDLE);
	void createImageViews();
	void createDepthResources();
	void createRenderPass();
	void createFramebuffers();
	void createSyncObjects();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkRenderPass renderPass;

	std::vector<VkImage> depthImages;
	std::vector<VkDeviceMemory> depthImageMemorys;
	std::vector<VkImageView> depthImageViews;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;


	Device& device;
	VkExtent2D windowExtent;
	Window& window;

	

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

};
