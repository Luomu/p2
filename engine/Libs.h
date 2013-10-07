#pragma once

#include "gl_core_4_3.h"

#include <SDL.h>
#undef WIN32_LEAN_AND_MEAN //will be redef by sdl_opengl.h
#include <SDL_opengl.h>
#include <SDL_assert.h>
#include <SDL_image.h> //the dependency is overkill, but we need versatile image loading

#include <DbgHelp.h> //for win32 stack trace
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <math.h>
#include <assert.h>
#include <map>

// GLM is our vector & matrix library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "contrib/pioneer/RefCounted.h"
#include "contrib/pioneer/FileSystem.h"
#include "contrib/pioneer/Color.h"