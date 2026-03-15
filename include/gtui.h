#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__ || __linux__ || __unix__ //__APPLE__ for macos and apple products, __linux__ for linux based systems (also android), __unix__ for everything else. Note that on clang __unix__ is not defined for historical reasons, even tho it is unix.
#include <termios.h>
#else
#error "Unknown operating system detected."
#endif

/* == ESCAPE SEQUENCES (use with printf == */
#define GTUI_ESC_CLEAR "\033[J"
#define GTUI_ESC_UP "\033[1A"
#define GTUI_ESC_DOWN "\033[1B"
#define GTUI_ESC_RIGHT "\033[1C"
#define GTUI_ESC_LEFT "\033[1D"
#define GTUI_ESC_DOWN_BEGIN "\033[1E" //Down and to the beginning of the line
#define GTUI_ESC_UP_BEGIN "\033[1F" //Up and to the beginning of the line

/* == API == */
void gtuiInitialize();

void gtuiMoveCursor(uint8_t x, uint8_t y);
void gtuiCleanCharacters(uint8_t x, uint8_t y, uint8_t count, char resetCursorPosition);

#ifdef GTUI_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
HANDLE outputHandle;
CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
#endif

#define GTUI_ASSERT(condition, message)\
{\
    if (!condition)\
    {\
        printf("\033[J""Assertion failed at line %d and file %s\n Details: %s\n", __LINE__, __FILE__, message);\
        exit(1);\
    }\
}

static char _initialized;

void gtuiInitialize()
{
    GTUI_ASSERT(!_initialized, "you've already initialized GTUI.");
    _initialized = 1;

    #ifdef _WIN32
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    //This lets me have the same code, for terminal input and output, for every OS
    SetConsoleMode(outputHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING); //Enable ANSI escape codes for windows terminal for output (color, cursor position)
    SetConsoleMode(outputHandle, ENABLE_VIRTUAL_TERMINAL_INPUT); //Same thing as above, only for user input
    #endif
}

void gtuiMoveCursor(uint8_t x, uint8_t y)
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI.");

	if (x > 99 || y > 99)
   	{
   		printf("gtuiMoveCursor: x and y must not be greater than 99.\n\tyou passed: x = %d, y = %d\n", x, y);
   		return;
   	}

   	char str[12];
   	snprintf(str, sizeof(str), "\033[%d;%dH", y, x);
   	
   	printf("%s", str);
}

void gtuiCleanCharacters(uint8_t x, uint8_t y, uint8_t count, char resetCursorPosition)
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI.");
    GTUI_ASSERT(count, "count must be greater than 1.");

	printf("TODO");
}

#endif
	
