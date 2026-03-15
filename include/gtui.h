#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#endif

#define GTUI_CLEAR_SCREEN() system("\033[J")

void gtuiInitialize();
void gtuiMoveCursor(uint8_t x, uint8_t y);
void gtuiCleanCharacters(uint8_t x, uint8_t y, uint8_t count, char resetCursorPosition);