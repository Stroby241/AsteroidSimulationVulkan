#pragma once
#include "App.h"

//struct GlobalUbo
//{
//	glm::mat4 projectionView{ 1.f };
//	glm::vec2 cursorPos{0,0};
//	glm::vec2 windowSize{ 0,0 };
//	glm::vec3 playerPos;
//};

class AppDemo : public App {
    
public:
    AppDemo();
	~AppDemo();

    AppDemo(const AppDemo&) = delete;
    AppDemo& operator=(const AppDemo&) = delete;

	void run() override;
    
private:
	void LoadModel();
    std::vector<GameObject> models;


};
