#include "VulkanRendering/AppDemo.h"
#include "Log.h"


int main() {

	Log::Init();
	App* app = new AppDemo();

	app->run();

    delete app;

    return 0;
}
