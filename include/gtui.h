#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__ || __linux__ || __unix__ //__APPLE__ for macos and apple products, __linux__ for linux based systems (also android), __unix__ for everything else. Note that on clang __unix__ is not defined for historical reasons, even tho it is unix.
#include <termios.h>
#include <sys/ioctl.h>
#else
#error "Unknown operating system detected."
#endif

/* == ESCAPE SEQUENCES (use with printf) == */

//Takes strings
#define GTUI_ESC_MOVE_CURSOR(x, y) "\033["x";"y"H"

#define GTUI_ESC_START "\033[0;0H"
#define GTUI_ESC_UP "\033[1A"
#define GTUI_ESC_DOWN "\033[1B"
#define GTUI_ESC_RIGHT "\033[1C"
#define GTUI_ESC_LEFT "\033[1D"
#define GTUI_ESC_DOWN_BEGIN "\033[1E" //Down and to the beginning of the line
#define GTUI_ESC_UP_BEGIN "\033[1F" //Up and to the beginning of the line

#define GTUI_ESC_ERASE_CURSOR_TO_END "\033[0J"
#define GTUI_ESC_ERASE_CURSOR_TO_BEGIN "\033[1J"
#define GTUI_ESC_ERASE_ALL "\033[2J"
#define GTUI_ESC_ERASE_CURSOR_TO_LINE_END "\033[0K"
#define GTUI_ESC_ERASE_CURSOR_TO_LINE_START "\033[1K"
#define GTUI_ESC_ERASE_LINE "\033[2K"

#define GTUI_ESC_RESET_STYLE "\033[0m"
#define GTUI_ESC_ENABLE_BOLD "\033[1m"
#define GTUI_ESC_DISABLE_BOLD "\033[22m"
#define GTUI_ESC_ENABLE_ITALIC "\033[3m"
#define GTUI_ESC_DISABLE_ITALIC "\033[23m"
#define GTUI_ESC_ENABLE_UNDERLINE "\033[4m"
#define GTUI_ESC_DISABLE_UNDERLINE "\033[24m"

#define GTUI_ESC_FG_BLACK "\033[30m"
#define GTUI_ESC_BG_BLACK "\033[40m"
#define GTUI_ESC_FG_RED "\033[31m"
#define GTUI_ESC_BG_RED "\033[41m"
#define GTUI_ESC_FG_GREEN "\033[32m"
#define GTUI_ESC_BG_GREEN "\033[42m"
#define GTUI_ESC_FG_YELLOW "\033[33m"
#define GTUI_ESC_BG_YELLOW "\033[43m"
#define GTUI_ESC_FG_BLUE "\033[34m"
#define GTUI_ESC_BG_BLUE "\033[44m"
#define GTUI_ESC_FG_MAGENTA "\033[35m"
#define GTUI_ESC_BG_MAGENTA "\033[45m"
#define GTUI_ESC_FG_CYAN "\033[36m"
#define GTUI_ESC_BG_CYAN "\033[46m"
#define GTUI_ESC_FG_WHITE "\033[37m"
#define GTUI_ESC_BG_WHITE "\033[47m"
#define GTUI_ESC_FG_DEFAULT "\033[39m"
#define GTUI_ESC_BG_DEFAULT "\033[49m"

#define GTUI_ESC_FG_BRIGHT_BLACK "\033[90m"
#define GTUI_ESC_BG_BRIGHT_BLACK "\033[100m"
#define GTUI_ESC_FG_BRIGHT_RED "\033[91m"
#define GTUI_ESC_BG_BRIGHT_RED "\033[101m"
#define GTUI_ESC_FG_BRIGHT_GREEN "\033[92m"
#define GTUI_ESC_BG_BRIGHT_GREEN "\033[102m"
#define GTUI_ESC_FG_BRIGHT_YELLOW "\033[93m"
#define GTUI_ESC_BG_BRIGHT_YELLOW "\033[103m"
#define GTUI_ESC_FG_BRIGHT_BLUE "\033[94m"
#define GTUI_ESC_BG_BRIGHT_BLUE "\033[104m"
#define GTUI_ESC_FG_BRIGHT_MAGENTA "\033[95m"
#define GTUI_ESC_BG_BRIGHT_MAGENTA "\033[105m"
#define GTUI_ESC_FG_BRIGHT_CYAN "\033[96m"
#define GTUI_ESC_BG_BRIGHT_CYAN "\033[106m"
#define GTUI_ESC_FG_BRIGHT_WHITE "\033[97m"
#define GTUI_ESC_BG_BRIGHT_WHITE "\033[107m"

#define GTUI_ESC_ENABLE_CURSOR "\033[?25h"
#define GTUI_ESC_DISABLE_CURSOR "\033[?25l"

/* == API == */
void gtuiInitialize();

void gtuiMoveCursor(uint8_t x, uint8_t y);
void gtuiGetConsoleSize(uint16_t* outX, uint16_t* outY);

#ifdef GTUI_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
HANDLE outputHandle;
HANDLE inputHandle;
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
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    DWORD dwOutMode = 0;
    DWORD dwInMode = 0;

    //This lets me have the same code, for terminal input and output, for every OS
    if (GetConsoleMode(outputHandle, &dwOutMode))
    {
        dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode |= DISABLE_NEWLINE_AUTO_RETURN;
        SetConsoleMode(outputHandle, dwOutMode);
    }

    if (GetConsoleMode(inputHandle, &dwInMode))
    {
        dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
        dwInMode &= ~ENABLE_LINE_INPUT;
        dwInMode &= ~ENABLE_ECHO_INPUT;
        SetConsoleMode(inputHandle, dwInMode);
    }
    #endif
}

void gtuiMoveCursor(uint16_t x, uint16_t y)
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI.");

	if (x > 999 || y > 999)
   	{
   		printf("gtuiMoveCursor: x and y must not be greater than 999.\n\tyou passed: x = %d, y = %d\n", x, y);
   		return;
   	}

   	char str[20];
   	snprintf(str, sizeof(str), "\033[%d;%dH\0", y, x);

   	printf("%s", str);
}

void gtuiGetConsoleSize(uint16_t* outX, uint16_t* outY)
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI.");
    GTUI_ASSERT(outX && outY, "gtuiGetConsoleSize: outX and outY must be valid pointers.");

    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    *outX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *outY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        *outX = w.ws_col;
        *outY = w.ws_row;
    #endif
}

#endif

