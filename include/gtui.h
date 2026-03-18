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

/* == ESCAPE SEQUENCES (write them to the stdout) == */

//Takes strings
#define GTUI_ESC_MOVE_CURSOR(x, y)         "\033["x";"y"H"
#define GTUI_ESC_TRANSLATE_CURSOR_UP(y)    "\033["y"A"
#define GTUI_ESC_TRANSLATE_CURSOR_DOWN(y)  "\033["y"B"
#define GTUI_ESC_TRANSLATE_CURSOR_RIGHT(x) "\033["x"C"
#define GTUI_ESC_TRANSLATE_CURSOR_LEFT(x)  "\033["x"D"

#define GTUI_ESC_START      "\033[1;1H"
#define GTUI_ESC_UP         "\033[1A"
#define GTUI_ESC_DOWN       "\033[1B"
#define GTUI_ESC_RIGHT      "\033[1C"
#define GTUI_ESC_LEFT       "\033[1D"
#define GTUI_ESC_DOWN_BEGIN "\033[1E" //Down and to the beginning of the line
#define GTUI_ESC_UP_BEGIN   "\033[1F" //Up and to the beginning of the line

#define GTUI_ESC_ERASE_CURSOR_TO_END        "\033[0J"
#define GTUI_ESC_ERASE_CURSOR_TO_BEGIN      "\033[1J"
#define GTUI_ESC_ERASE_ALL                  "\033[2J"
#define GTUI_ESC_ERASE_CURSOR_TO_LINE_END   "\033[0K"
#define GTUI_ESC_ERASE_CURSOR_TO_LINE_START "\033[1K"
#define GTUI_ESC_ERASE_LINE                 "\033[2K"
#define GTUI_ESC_ERASE_CHAR                 "\033[X" //Erases char at cursor position
#define GTUI_ESC_DELETE_CHAR                "\033[P" //Erases char at cursor position, and moves everything back in the current line.

#define GTUI_ESC_RESET_STYLE       "\033[0m"
#define GTUI_ESC_ENABLE_BOLD       "\033[1m"
#define GTUI_ESC_DISABLE_BOLD      "\033[22m"
#define GTUI_ESC_ENABLE_ITALIC     "\033[3m"
#define GTUI_ESC_DISABLE_ITALIC    "\033[23m"
#define GTUI_ESC_ENABLE_UNDERLINE  "\033[4m"
#define GTUI_ESC_DISABLE_UNDERLINE "\033[24m"

#define GTUI_ESC_FG_BLACK   "\033[30m"
#define GTUI_ESC_BG_BLACK   "\033[40m"
#define GTUI_ESC_FG_RED     "\033[31m"
#define GTUI_ESC_BG_RED     "\033[41m"
#define GTUI_ESC_FG_GREEN   "\033[32m"
#define GTUI_ESC_BG_GREEN   "\033[42m"
#define GTUI_ESC_FG_YELLOW  "\033[33m"
#define GTUI_ESC_BG_YELLOW  "\033[43m"
#define GTUI_ESC_FG_BLUE    "\033[34m"
#define GTUI_ESC_BG_BLUE    "\033[44m"
#define GTUI_ESC_FG_MAGENTA "\033[35m"
#define GTUI_ESC_BG_MAGENTA "\033[45m"
#define GTUI_ESC_FG_CYAN    "\033[36m"
#define GTUI_ESC_BG_CYAN    "\033[46m"
#define GTUI_ESC_FG_WHITE   "\033[37m"
#define GTUI_ESC_BG_WHITE   "\033[47m"
#define GTUI_ESC_FG_DEFAULT "\033[39m"
#define GTUI_ESC_BG_DEFAULT "\033[49m"

#define GTUI_ESC_FG_BRIGHT_BLACK   "\033[90m"
#define GTUI_ESC_BG_BRIGHT_BLACK   "\033[100m"
#define GTUI_ESC_FG_BRIGHT_RED     "\033[91m"
#define GTUI_ESC_BG_BRIGHT_RED     "\033[101m"
#define GTUI_ESC_FG_BRIGHT_GREEN   "\033[92m"
#define GTUI_ESC_BG_BRIGHT_GREEN   "\033[102m"
#define GTUI_ESC_FG_BRIGHT_YELLOW  "\033[93m"
#define GTUI_ESC_BG_BRIGHT_YELLOW  "\033[103m"
#define GTUI_ESC_FG_BRIGHT_BLUE    "\033[94m"
#define GTUI_ESC_BG_BRIGHT_BLUE    "\033[104m"
#define GTUI_ESC_FG_BRIGHT_MAGENTA "\033[95m"
#define GTUI_ESC_BG_BRIGHT_MAGENTA "\033[105m"
#define GTUI_ESC_FG_BRIGHT_CYAN    "\033[96m"
#define GTUI_ESC_BG_BRIGHT_CYAN    "\033[106m"
#define GTUI_ESC_FG_BRIGHT_WHITE   "\033[97m"
#define GTUI_ESC_BG_BRIGHT_WHITE   "\033[107m"

#define GTUI_ESC_ENABLE_CURSOR  "\033[?25h"
#define GTUI_ESC_DISABLE_CURSOR "\033[?25l"

#define GTUI_ESC_ALT_SCREEN_ENABLE  "\033[?1049h" //Enable alternative screen buffer (basically write on a new SCROLL-LESS canvas (no scroll history, simple 2d grid))
#define GTUI_ESC_ALT_SCREEN_DISABLE "\033[?1049l" //Disable the alternative screen buffer (restore the previous buffer)

typedef enum
{
    GTUI_EVENT_KEY,

    GTUI_EVENT_BACKSPACE, //Even tho it should be a KEY, it is a special one, not printable. The same applies for the others in the block below
    GTUI_EVENT_ENTER,
    GTUI_EVENT_ESCAPE,

    GTUI_EVENT_RESIZE,

    GTUI_EVENT_ESC_UP,
    GTUI_EVENT_ESC_DOWN,
    GTUI_EVENT_ESC_RIGHT,
    GTUI_EVENT_ESC_LEFT,

    GTUI_EVENT_NULL //If there was no event in this cycle (returned on unknown events or null reads on non-blocking input cycles).
} GTUIEventType;

typedef struct
{
    GTUIEventType type;
    char key; //represent an ASCII printable character, if the event type is GTUI_EVENT_KEY.
} GTUIEvent;

#ifdef __cplusplus
extern "C"
{
#endif

/* == API == */
void gtuiInitialize();
void gtuiTerminate();

void gtuiMoveCursor(uint16_t x, uint16_t y);
void gtuiGetConsoleSize(uint16_t* outX, uint16_t* outY);

GTUIEvent gtuiGetInput();

void gtuiSetBlockingInput(char block); //Do you want to enable blocking input?

#ifdef __cplusplus
}
#endif

/* == IMPLEMENTATION == */
#ifdef GTUI_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <fcntl.h>
#include <conio.h>
#include <io.h>
#endif

#define GTUI_ASSERT(condition, message)\
{\
    if (!condition)\
    {\
        printf("\033[J""Assertion failed at line %d and file %s\n Details: %s\n", __LINE__, __FILE__, message);\
        exit(1);\
    }\
}

#ifdef _WIN32
static HANDLE _outputHandle;
static HANDLE _inputHandle;
static CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBufferInfo;

//I'll use these to reset the console state once the program ends.
static DWORD _originalOutputMode;
static DWORD _originalInputMode;
#else
static struct termios originalTermiosConf;
#endif

static char _isInputBlocking = 1;
static char _initialized;

void gtuiInitialize()
{
    GTUI_ASSERT(!_initialized, "you've already initialized GTUI.");
    _initialized = 1;

    #ifdef _WIN32
    _outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    _inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    DWORD dwOutMode = 0;
    DWORD dwInMode = 0;

    //Making this terminal behave like a VT100 which supports Escape Codes and is the de facto standard of terminals, so its super portable.
    //This lets me have the same code, for terminal input and output, for every OS
    if (GetConsoleMode(_outputHandle, &dwOutMode))
    {
        _originalOutputMode = dwOutMode;

        dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode |= DISABLE_NEWLINE_AUTO_RETURN;
        SetConsoleMode(_outputHandle, dwOutMode);
    }

    //Enable Raw Mode (no input buffering)
    //Everything will be read with _read() in Windows and read() in unix
    if (GetConsoleMode(_inputHandle, &dwInMode))
    {
        _originalInputMode = dwInMode;

        dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
        dwInMode &= ~ENABLE_LINE_INPUT;
        dwInMode &= ~ENABLE_ECHO_INPUT;
        dwInMode &= ~ENABLE_PROCESSED_INPUT;
        dwInMode |= ENABLE_EXTENDED_FLAGS;
        dwInMode &= ~ENABLE_QUICK_EDIT_MODE;
        SetConsoleMode(_inputHandle, dwInMode);
    }

    _setmode(_fileno(stdin), _O_BINARY); //Removing CRLF windows mode and keeping the stream to binary data
    #else
    //Enable Raw Mode for unix
    tcgetattr(STDIN_FILENO, &originalTermiosConf);
    struct termios raw = originalTermiosConf;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1; //Minimum of bytes to expect from read() (more than 0 means blocking)
    raw.c_cc[VTIME] = 0; //How much time before read() timeout (1 = 100ms)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
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
   	snprintf(str, sizeof(str), "\033[%d;%dH\0", y + 1, x + 1);

   	fwrite(str, 1, strlen(str), stdout);
}

void gtuiGetConsoleSize(uint16_t* outX, uint16_t* outY)
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI.");
    GTUI_ASSERT(outX && outY, "gtuiGetConsoleSize: outX and outY must be valid pointers.");

    #ifdef _WIN32
    GetConsoleScreenBufferInfo(_outputHandle, &_consoleScreenBufferInfo);

    *outX = _consoleScreenBufferInfo.srWindow.Right - _consoleScreenBufferInfo.srWindow.Left + 1;
    *outY = _consoleScreenBufferInfo.srWindow.Bottom - _consoleScreenBufferInfo.srWindow.Top + 1;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        *outX = w.ws_col;
        *outY = w.ws_row;
    #endif
}

void gtuiTerminate()
{
    GTUI_ASSERT(_initialized, "you must first initialize GTUI to call gtuiTerminate().");

    #ifdef _WIN32
    SetConsoleMode(_outputHandle, _originalOutputMode);
    SetConsoleMode(_inputHandle, _originalInputMode);
    #else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermiosConf);
    #endif

    _initialized = 0;
}

void gtuiSetBlockingInput(char block)
{
    //Windows will manage this directly in the event polling function

    if (_isInputBlocking == block)
        return;
    _isInputBlocking = !_isInputBlocking;

    #ifndef _WIN32
    struct termios conf;
    tcgetattr(STDIN_FILENO, &conf);
    conf.c_cc[VMIN] = _isInputBlocking == 1;
    #endif
}

GTUIEvent gtuiGetInput()
{
    #ifdef _WIN32
    //_kbhit might have stored in its buffer some chars that Windows event queue has already cleared, because _kbhit could've grabbed it all in 1 operation while the actual bytes might be more.
    if (!_kbhit())
    {
        if (WaitForSingleObject(_inputHandle, _isInputBlocking ? INFINITE : 0) != WAIT_OBJECT_0)
        {
            return (GTUIEvent) { GTUI_EVENT_NULL, 0 };
        }
    }

    //The event is a key
    if (_kbhit())
    {
        char c;
        _read(_fileno(stdin), &c, 1);

        //It is an escape char, or escape sequence
        if (c == '\033')
        {
            //If there is another character in the stream
            if (WaitForSingleObject(_inputHandle, 5 /*Let's wait a few milliseconds for the next char in the sequence*/) == WAIT_OBJECT_0 && _kbhit())
            {
                char sequence[2] = { 0 };
                _read(_fileno(stdin), sequence, 1);

                if (sequence[0] == '[' &&
                    WaitForSingleObject(_inputHandle, 5 /*Let's wait a few milliseconds for the next char in the sequence*/) == WAIT_OBJECT_0 &&
                    _kbhit())
                {
                    _read(_fileno(stdin), sequence + 1, 1);
                }

                switch (sequence[1])
                {
                case 'A': return (GTUIEvent) { GTUI_EVENT_ESC_UP,    0 };
                case 'B': return (GTUIEvent) { GTUI_EVENT_ESC_DOWN,  0 };
                case 'C': return (GTUIEvent) { GTUI_EVENT_ESC_RIGHT, 0 };
                case 'D': return (GTUIEvent) { GTUI_EVENT_ESC_LEFT,  0 };
                default:  return (GTUIEvent) { GTUI_EVENT_NULL,      0 }; //Unknown escape sequence
                }
            }

            //If not it is an escape char
            else
            {
                return (GTUIEvent){ GTUI_EVENT_ESCAPE, 0 };
            }
        }

        else
        {
            switch (c)
            {
            case '\r': return (GTUIEvent) { GTUI_EVENT_ENTER,     0 };
            case 127:  return (GTUIEvent) { GTUI_EVENT_BACKSPACE, 0 };
            case 8:    return (GTUIEvent) { GTUI_EVENT_BACKSPACE, 0 };
            default:   return (GTUIEvent) { GTUI_EVENT_KEY,       c };
            }
        }
    }

    //An event has arrived, but no key was pressed, it was a different event type
    else
    {
        INPUT_RECORD ir;
        DWORD readCount;
        ReadConsoleInput(_inputHandle, &ir, 1, &readCount);

        if (ir.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            return (GTUIEvent) { GTUI_EVENT_RESIZE, 0 };
        }

        else
        {
            return (GTUIEvent) { GTUI_EVENT_NULL, 0 }; //Unknown event
        }
    }
    #else
    //unix is to be implemented.
    #endif
}
#endif

