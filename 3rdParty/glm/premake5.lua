project "glm"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

	flags "MultiProcessorCompile"

    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"
    filter {}

	targetdir "%{wks.location}/build/3rdParty/bin/%{prj.name}"
	objdir "%{wks.location}/build/3rdParty/bin-int/%{prj.name}"

    files {
        "glm/**.cpp",
        "glm/**.h"
    }

    includedirs {
        "../glm"
    }    