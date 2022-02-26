#include "render_system.h"
#include "Device.h"
#include "Renderer.h"

struct GuiData {
    bool startPhysic;
    float speedPhysic;
    bool lockPosPhysic;
};

class Gui {
public:
	Gui(Device& device,Window& window , Renderer& renderer, GuiData& data);
	~Gui();
	
	enum class Gui_Draw : uint8_t 
	{
		None = 0,
		MainMenu = 1 << 0,
	};
    
    bool enableDockingSpace = true;
    
	void update(FrameInfo frameInfo);

	int GuiToDraw = 0;

private:
	Device& device;
	Renderer& renderer;
	Window& window;
	GuiData& data;

	void InitImGui();
	VkDescriptorPool imguiPool;
    
    
    void DockingSpace();
    void DrawInGameMenu();
};
