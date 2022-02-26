#include "Window.h"
#include "Log.h"
#include "Player/Player.h"
#include <iostream>

Window::Window(int width, int height, const char* name)
	: width(width), height(height), windowName(name)
{
	initWindow();
	keys = new bool[400];
	for (int i = 0; i < 400; i++)
	{
		keys[i] = false;
	}
}

Window::~Window()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		VK_LOG_ERROR("failed to create window surface!");
		exit(0);
	}
}


static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
	app->width = width;
	app->height = height;
	if (width > 0 && height > 0) {
		app->player->camera.SetViewSize(width, height);
	}
}

static bool mouseFree = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS) {
        app->keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        app->keys[key] = false;
    }
    if(app->keys[GLFW_KEY_TAB]){
        mouseFree = !mouseFree;
        app->keys[GLFW_KEY_TAB] = false;
        if(mouseFree){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}


static void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(!mouseFree){
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->player->camera.onMouseMoved(app->width / 2 - xpos, app->height / 2 - ypos);
        app->player->camera.update();
        glfwSetCursorPos(window, app->width/2, app->height/2);
    }
}


void Window::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    int monitor_width, monitor_height;
	const GLFWvidmode* videoMdoe = glfwGetVideoMode(monitor);

	if (width > videoMdoe->width) {
		width = videoMdoe->width;
	}
	if (height > videoMdoe->height) {
		height = videoMdoe->height;
	}
	
    glfwGetMonitorPhysicalSize(monitor,&monitor_width,&monitor_height);

	window = glfwCreateWindow(width, height, windowName, nullptr , nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
}
