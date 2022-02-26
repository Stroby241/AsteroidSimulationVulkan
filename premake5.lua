
LIBS = {}

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
    }

    architecture "x64"
    cppdialect "C++20"

    includedirs {
        "src"
    }

    libdirs
    {

    }

    links {
 
    }

    defines{

    }

    filter "configurations:Debug" 
        defines { "DEBUG" } 
        prebuildcommands { 
           
        }
        symbols "On"

    filter "configurations:Release" 
        defines { "NDEBUG" }      
        optimize "On"
        