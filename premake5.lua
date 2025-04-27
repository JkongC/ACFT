workspace "ACFT"
  configurations { "Debug", "Release" }
  system "Windows"
  architecture "x64"
  cppdialect "C++20"
  systemversion "latest"

  startproject "Sandbox"
  
  -- 全局模块输出目录
  moduleoutdir = "%{wks.location}/ifc"

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

  -- defines { "ACFT_EXPORTS" }
  
  -- 源文件配置（保持原始结构）
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
    --"{MKDIR} ../ifc"
    --"{COPYFILE} %{cfg.objdir}*.ifc ../ifc"
  }
  
  filter { "files:src/**.ixx" }
    compileas "Module"
    buildoutputs { 
      "%{moduleoutdir}/%{file.basename}.ifc",
      "%{cfg.objdir}/%{file.basename}.obj" 
    }
  
  filter { "configurations:Debug" }
    defines { "NDEBUG", "ACFT_DEBUG" }
    optimize "Off"
  
  filter { "configurations:Release" }
    defines { "NDEBUG" }
    optimize "Speed"


project "Sandbox"
  kind "ConsoleApp"
  language "C++"
  location "Sandbox"
  targetdir "bin"

  files {
    "%{prj.name}/src/**.ixx",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/src/**.h"
    -- "ACFT/src/**.ixx"
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
  

  local ifc_files = os.matchfiles("ACFT/src/**.ixx")
  local ref_options = {}

  for _, f in ipairs(ifc_files) do
    table.insert(ref_options, "/reference \"%{wks.location}/ifc/" .. path.getname(f) .. ".ifc\"")
  end

  buildoptions (ref_options)
  
  postbuildcommands {
    "{COPYFILE} ../lib/*.dll %{cfg.targetdir}/"
  }