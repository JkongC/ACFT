workspace "ACFT"
  configurations { "Dist", "Release", "Debug" }
  system "Windows"
  architecture "x64"
  cppdialect "C++20"
  systemversion "latest"

  startproject "Sandbox"

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

  libdirs { "lib" }

  filter { "action:vs*" }
    buildoptions {
      "/utf-8"
    }
  
project "ACFT"
  kind "StaticLib"
  language "C++"
  location "ACFT"
  targetdir "bin"
  
  files {
    "%{prj.name}/src/**.ixx",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/src/**.h"
  }

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  links {
    "opengl32",
    "glew32",
    "glfw3",
    "spdlog",
    "fmt"
  }

  includedirs {
    "ACFT/src",
    "ACFT/vendor",
    "ACFT/vendor/glm",
    "ACFT/vendor/entt/src",
  }

  postbuildcommands {
    "{MKDIR} ../bin/" .. outputdir .. "/Sandbox/",
    "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/%{cfg.buildtarget.name}"
  }

  filter { "files:ACFT/src/Config/Config-Thread.ixx" }
    defines { "ACFT_RENDER_THREAD", "ACFT_EVENT_THREAD" }
  
  filter { "configurations:Debug" }
    defines { "NDEBUG", "ACFT_DEBUG", "ACFT_ENABLE_LOG" }
    optimize "Off"
  
  filter { "configurations:Release" }
    defines { "NDEBUG", "ACFT_ENABLE_LOG" }
    optimize "Speed"

  filter { "configurations:Dist" }
    defines { "NDEBUG" }
    optimize "Speed"

  filter { "toolset:msc*" }
    linkoptions { "/IGNORE:4006" }


project "Sandbox"
  kind "ConsoleApp"
  language "C++"
  location "Sandbox"
  targetdir "bin"

  files {
    "%{prj.name}/src/**.ixx",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/src/**.h"
  }

  includedirs {
    "ACFT/src",
    "ACFT/vendor",
    "ACFT/vendor/glm",
    "ACFT/vendor/entt/src",
  }

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  links { "ACFT" }
  
  postbuildcommands {
    "{COPYFILE} ../lib/*.dll %{cfg.targetdir}/",
    "{MKDIR} %{cfg.targetdir}/resources/imgs/",
    "{MKDIR} %{cfg.targetdir}/resources/shaders/",
    "{COPYFILE} resources/imgs/* %{cfg.targetdir}/resources/imgs/",
    "{COPYFILE} resources/shaders/* %{cfg.targetdir}/resources/shaders/"
  }