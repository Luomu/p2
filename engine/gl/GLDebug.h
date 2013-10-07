#pragma once
#include "engine/Libs.h"

namespace GlDebug {
	void Init();
	void AddMessage(const std::string&);

	void PrintStack();
	void __stdcall DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, const void *userParam);

	/// Helper functions to print human-readable message
	std::string getStringForSource(GLenum source);
	std::string getStringForType(GLenum type);
	std::string getStringForSeverity(GLenum type);

	extern void *spUserParam;
};