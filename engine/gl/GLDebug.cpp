#include "GlDebug.h"
#include "engine/OS.h"

void *GlDebug::spUserParam = nullptr;
std::ostream *outStream = nullptr;

void GlDebug::Init()
{
	OS::Log("GLDebug init");
	outStream = (std::iostream *)&std::cout;

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugLog, spUserParam);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, false);
}

void GlDebug::AddMessage(const std::string &msg)
{
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
		GL_DEBUG_TYPE_MARKER, 666, GL_DEBUG_SEVERITY_NOTIFICATION,
		GLsizei(msg.size()), msg.c_str());
}

void GlDebug::DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, const void *userParam)
{
	 *outStream << " -- \n" << "Type: " <<
		getStringForType(type).c_str() << "; Source: " <<
		getStringForSource(source).c_str() <<"; ID: " << id << "; Severity: " <<
		getStringForSeverity(severity).c_str() << "\n" << message << "\n";

	if (severity == GL_DEBUG_SEVERITY_HIGH)
		OS::PrintStackTrace();
}

// aux function to translate source to string
std::string
GlDebug::getStringForSource(GLenum source) {
	switch(source) {
		case GL_DEBUG_SOURCE_API:
			return("API");
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return("Window System");
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return("Shader Compiler");
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return("Third Party");
		case GL_DEBUG_SOURCE_APPLICATION:
			return("Application");
		case GL_DEBUG_SOURCE_OTHER:
			return("Other");
		default:
			return("Dunno");
	}
}

// aux function to translate severity to string
std::string
GlDebug::getStringForSeverity(GLenum severity) {
	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			return("High");
		case GL_DEBUG_SEVERITY_MEDIUM:
			return("Medium");
		case GL_DEBUG_SEVERITY_LOW:
			return("Low");
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return("Notification");
		default:
			return("");
	}
}

// aux function to translate type to string
std::string
GlDebug::getStringForType(GLenum type) {
	switch(type) {
		case GL_DEBUG_TYPE_ERROR:
			return("Error");
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return("Deprecated Behaviour");
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return("Undefined Behaviour");
		case GL_DEBUG_TYPE_PORTABILITY:
			return("Portability Issue");
		case GL_DEBUG_TYPE_PERFORMANCE:
			return("Performance Issue");
		case GL_DEBUG_TYPE_OTHER:
			return("Other");
		case GL_DEBUG_TYPE_MARKER:
			return("Marker");
		default:
			return("");
	}
}