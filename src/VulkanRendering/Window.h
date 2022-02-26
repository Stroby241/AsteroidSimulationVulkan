#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Player;

class Window {
public:
	Window(int width, int height, const char* name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	GLFWwindow* GetGLFWWindow() const { return  window; }

	VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

	bool framebufferResized = false;

	int width;
	int height;

	bool* keys;

	Player* player;

private:
	void initWindow();

	const char* windowName;
	GLFWwindow* window;

};
