#pragma once

namespace OS {
	///show a message box
	void Warning(const char* format, ...);
	///show a message box and abort
	void Error(const char* format, ...);
	///trace to console and/or log file
	void Log(const char* format, ...);

	///print stack trace, if possible
	void PrintStackTrace();

	///set up custom exception handler, if possible
	void SetUpExceptionHandler();

	///call at application shutdown: dump memory leaks to console,
	///if possible
	void DumpMemoryLeaks();
}