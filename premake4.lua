--This is VS oriented but it's not a big effort to add
--configurations for other platforms.
solution "p2"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	flags {
		"Symbols",
		"Unicode",
		"FatalWarnings",
		"NoEditAndContinue"
	}
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
	--disable some VS specific warnings
	--try to provide some rationale
	buildoptions {
		"/wd4244", --conversion from x to x, don't care on gcc
		"/wd4800", --int-to-bool, don't care on gcc
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

	project "Lua"
		kind "StaticLib"
		files { "contrib/lua/*" }
		excludes { "contrib/lua/lua.c", "contrib/lua/luac.c" }

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
