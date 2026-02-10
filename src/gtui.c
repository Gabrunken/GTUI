#include <gtui.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
HANDLE outputHandle;
CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;

#define CLEAR_SCREEN() system("cls")

#else

#define CLEAR_SCREEN() system("clear")

#endif

#define ASSERT(condition, message)\
{\
    if (!condition)\
    {\
        CLEAR_SCREEN();\
        printf("Assertion failed at line %d and file %s\n Details: %s\n", __LINE__, __FILE__, message);\
        exit(1);\
    }\
}

static char initialized;

void Initialize()
{
    ASSERT(!initialized, "you've already initialized GTUI.");
    initialized = 1;

    #ifdef _WIN32
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    #else
    #endif
}

void MoveCursor(uint8_t x, uint8_t y)
{
    ASSERT(initialized, "you must first initialize GTUI.");

    #ifdef _WIN32
    SetConsoleCursorPosition(outputHandle, (COORD){x, y});
    #else
    #endif
}

void CleanCharacters(uint8_t x, uint8_t y, uint8_t count, char resetCursorPosition)
{
    ASSERT(initialized, "you must first initialize GTUI.");
    ASSERT(count, "count must be greater than 1.");

    #ifdef _WIN32
    //Save old position
    ASSERT(GetConsoleScreenBufferInfo(outputHandle, &consoleScreenBufferInfo), "failed to retrieve console info.");

    SetConsoleCursorPosition(outputHandle, (COORD){x, y});

    if (count == 1)
    {
        fwrite(" ", 1, 1, stdout);
    }

    else
    {
        char buf[count];
        memset(buf, ' ', count);
        fwrite(buf, sizeof(buf), 1, stdout);
    }

    if (resetCursorPosition)
    {
        SetConsoleCursorPosition(
        outputHandle,
        (COORD){consoleScreenBufferInfo.dwCursorPosition.X, consoleScreenBufferInfo.dwCursorPosition.Y});
    }
    #else
    #endif
}