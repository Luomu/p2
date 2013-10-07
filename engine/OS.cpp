#include "OS.h"
#include "Libs.h"

void OS::Warning(const char *fmt, ...) {
	char buf[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Warning", buf, nullptr);
}

void OS::Error(const char *fmt, ...) {
	char buf[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", buf, nullptr);
	exit(1);
}

void OS::Log(const char *fmt, ...) {
	char buf[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	const Uint32 ms = SDL_GetTicks();
	char timestamp[16] = {};
	std::sprintf(timestamp, "%u:%u:%u",
		(ms / (1000*60*60)) % 24,
		(ms / (1000*60)) % 60,
		(ms / 1000) % 60);

	std::cout << timestamp << " - " << buf << std::endl;
}