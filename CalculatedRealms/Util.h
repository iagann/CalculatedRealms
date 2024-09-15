#pragma once

#include <iostream>
#include <windows.h>

#include <omp.h>

class Util {
public:
    // Function to set console text color
    static void SetConsoleColor(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
    static // Function to reset console color to default
        void ResetConsoleColor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Standard white
    }
    static int getMaxThreads() { return omp_get_max_threads(); }
};