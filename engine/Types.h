#pragma once
#include "Libs.h"

typedef signed char    S8;
typedef unsigned char  U8;
typedef signed short   S16;
typedef unsigned short U16;
typedef signed int     S32;
typedef unsigned int   U32;

const double PI = 3.14159265358979323846;

using glm::ivec2;
using glm::ivec4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::quat;

// Graphics types

enum TEXTURE_FORMAT {
	TEXTURE_NONE,
	TEXTURE_RGB_888,
	TEXTURE_RGBA_8888,
	TEXTURE_SRGB_888,
	TEXTURE_SRGBA_8888,
	TEXTURE_DXT1,
	TEXTURE_DXT5,
	TEXTURE_DEPTH_24
};

enum SHADER_CONST_TYPE {
	SHADER_CONST_NONE,
	SHADER_CONST_FLOAT,
	SHADER_CONST_FLOAT2,
	SHADER_CONST_FLOAT3,
	SHADER_CONST_FLOAT4,
	SHADER_CONST_MAT4
};

enum BLEND_MODE {
	BLEND_NONE,
	BLEND_ADDITIVE,
	BLEND_ALPHA,
	BLEND_ALPHA_PREMULT
};

enum CULL_MODE {
	CULL_NONE,
	CULL_FRONT, //CW
	CULL_BACK   //CCW
};

enum FILL_MODE {
	FILL_SOLID,
	FILL_WIREFRAME
};

enum BUFFER_USAGE {
	/// updated once during lifetime
	BUFFER_STATIC,
	/// updated several times during lifetime
	BUFFER_DYNAMIC
};

enum ATTRIB_SEMANTIC {
	ATTRIB_NONE,
	ATTRIB_POSITION,
	ATTRIB_COLOR,
	ATTRIB_NORMAL,
	ATTRIB_UV
};

enum ATTRIB_FORMAT {
	ATTRIB_FORMAT_NONE,
	ATTRIB_FORMAT_FLOAT2,
	ATTRIB_FORMAT_FLOAT3,
	ATTRIB_FORMAT_FLOAT4,
	ATTRIB_FORMAT_UBYTE4
};

enum TEXTURE_FILTER_MODE {
	///linear magnification, nearest mipmap filtering
	TEXTURE_FILTER_BILINEAR,
	///linear magnification, linear mipmap filtering
	TEXTURE_FILTER_TRILINEAR,
	///point sampling for minification & magnification
	TEXTURE_FILTER_NEAREST
};

enum TEXTURE_ADDRESS_MODE {
	TEXTURE_ADDRESS_WRAP,
	TEXTURE_ADDRESS_CLAMP
};

enum PRIMITIVE_TYPE {
	PRIMITIVE_TRIANGLES,
	PRIMITIVE_TRIANGLE_STRIP,
};

// End graphics types