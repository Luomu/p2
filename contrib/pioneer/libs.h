#pragma once
//minimal includes to get Pioneer files building

#pragma warning(disable : 4267) //conversion from size_t to int - we don't care
#pragma warning(disable : 4244) //same
#pragma warning(disable : 4800) //forcing int to bool - don't care

#include "engine/Libs.h"
#include "engine/Utils.h"
#include "Color.h"
#include "fixed.h"
#include "FloatComparison.h"
#include "text/TextSupport.h"
#include "LuaUtils.h"

typedef SDL_Keycode SDLKey;
typedef SDL_Keymod  SDLMod;
#define KMOD_META KMOD_GUI
typedef glm::vec3 vector3f;
typedef glm::vec2 vector2f;

#undef RegisterClass //windows.h confusing lua