#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#endif

void Initialize();
void MoveCursor(uint8_t x, uint8_t y);
void CleanCharacters(uint8_t x, uint8_t y, uint8_t count, char resetCursorPosition);