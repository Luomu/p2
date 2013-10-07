solution "p2"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	flags { "Symbols", "Unicode" }
	language "C++"
	defines { "_CRT_SECURE_NO_WARNINGS" }
	includedirs {
		"../p2-thirdparty/sdl/include",
		"../p2-thirdparty/sdl_image/include",
		"../p2-thirdparty/ovr/include",
		"../p2-thirdparty/libsigc++/include",
		"../p2-thirdparty",
		"."
	}
	libdirs {
		"../p2-thirdparty/sdl/lib",
		"../p2-thirdparty/sdl_image/lib",
		"../p2-thirdparty/ovr/lib",
		"../p2-thirdparty/libsigc++/lib",
	}

	--project definitions may be split into sub .luas
	--as the list grows
	project "Test"
		targetname "Test"
		kind "ConsoleApp"
		links { "Engine", "Lua" }
		links {
			"SDL2", "SDL2main", "SDL2_image",
			"opengl32",
			"libovr64",
			"sigc-vc100-2_0"
		}
		links {
			"Shlwapi", --win32 path handling
			"dbghelp", --win32 stack trace
			"winmm",   --libOVR dependency
		}
		linkoptions {
			"/NODEFAULTLIB:msvcrt.lib"
		}
		includedirs { "contrib" }
		includedirs { "contrib/pioneer" }
		files { "test.cpp" }

	project "Engine"
		kind "StaticLib"
		files {
			"engine/*", "engine/gl/*", "engine/win32/*",
		}
		files {
			"contrib/pioneer/*",
			"contrib/pioneer/win32/*",
			"contrib/pioneer/text/*",
			"contrib/jenkins/*"
		}
		includedirs { "contrib", "contrib/pioneer", "engine/ui_compat" }
