
LIBS = {}
LIBS["spdlog"] = "vendor/spdlog"
LIBS["glm"] = "vendor/glm"
LIBS["vulkan"] = "C:/VulkanSDK/1.3.204.0"
LIBS["imgui"] = "vendor/imgui"
LIBS["glfw"] = "vendor/glfw-3.3.4.bin.WIN64"
LIBS["imgui"] = "vendor/imgui"
LIBS["boost"] = "vendor/boost"

workspace "AsteroidSimulationVulkan" 
    configurations { "Debug","Release" } 
    
project "AsteroidSimulationVulkan"   
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    
    files { 
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "vendor/imgui/**.cpp",
    }

    architecture "x64"
    cppdialect "C++20"

    includedirs {
        "src",
        "%{LIBS.glm}",
        "%{LIBS.spdlog}",
        "%{LIBS.glfw}/include",
        "%{LIBS.imgui}",
        "%{LIBS.vulkan}/Include",
        "%{LIBS.boost}",
    }

    libdirs
    {
        "%{LIBS.vulkan}/Lib",
        "%{LIBS.glfw}/lib-vc2019",
    }

    links {
        "glfw3",
        "vulkan-1",
    }

    defines{

    }

    filter "configurations:Debug" 
        defines { "DEBUG" } 
        symbols "On"

    filter "configurations:Release" 
        defines { "NDEBUG" }      
        optimize "On"
        