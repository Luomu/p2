#include "engine/OS.h"
#include <crtdbg.h>
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>
#pragma comment(lib, "Dbghelp")

void OS::PrintStackTrace()
{
	void *stack[100];

	HANDLE process = GetCurrentProcess();

	SymSetOptions(SYMOPT_LOAD_LINES);
	SymInitialize(process, NULL, TRUE);

	unsigned short frames = CaptureStackBackTrace(0, 200, stack, NULL);
	SYMBOL_INFO *symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for (size_t i = 0; i < frames; i++) {
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
		DWORD displacement;
		IMAGEHLP_LINE64 line;

		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		if (!strstr(symbol->Name, "OS::") &&
			SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacement, &line)) {
				Log("-- %s, line %u", symbol->Name, line.LineNumber);
		}
		if (strcmp(symbol->Name, "main") == 0) break;
	}

	free(symbol);
}

LONG WINAPI crash_handler(EXCEPTION_POINTERS*)
{
	OS::Log("Unhandled exception, terminating.");
	OS::PrintStackTrace();
	return EXCEPTION_EXECUTE_HANDLER;
}

void OS::SetUpExceptionHandler()
{
	SetUnhandledExceptionFilter(crash_handler);
}

void OS::DumpMemoryLeaks()
{
	_CrtDumpMemoryLeaks();
}