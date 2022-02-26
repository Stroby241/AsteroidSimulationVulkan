#include "VulkanRendering/App.h"
#include "Log.h"


int main() {

	Log::Init();
	App* app = new App();

	app->run();

    delete app;

    return 0;
}
