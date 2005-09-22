/*
 * win32test.c : Test basic library functions under Win32
 *
 * This file is a part of the OpenInput library.
 * Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ******************************************************************** */

// Includes
#include "openinput.h"
#include <windows.h>
#include <stdio.h>

// Platform independent test function
void platform_test();

/* ******************************************************************** */

// Global vars
HINSTANCE instance;
HWND window;

/* ******************************************************************** */

// Window procedure
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg,
                             WPARAM wparam, LPARAM lparam) {
    // We only want the close/destroy message
    if((msg == WM_CLOSE) || (msg == WM_DESTROY)) {
        fprintf(stderr, "win32test: catched close/destroy\n");
        PostQuitMessage(0);
        return 0;
    }

    // Just do default handling
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

/* ******************************************************************** */

// Initialize window class
int init_class(HINSTANCE inst) {
    WNDCLASSEX wcx;

    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = window_proc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = inst;
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = GetStockObject(WHITE_BRUSH);
    wcx.lpszMenuName =  "OpenInputMenu";
    wcx.lpszClassName = "OpenInputTest";
    wcx.hIconSm = LoadImage(inst,
                            MAKEINTRESOURCE(5),
                            IMAGE_ICON,
                            GetSystemMetrics(SM_CXSMICON),
                            GetSystemMetrics(SM_CYSMICON),
                            LR_DEFAULTCOLOR);

    return RegisterClassEx(&wcx);
}

/* ******************************************************************** */

// Create window
int init_window(HINSTANCE inst, int showcmd) {
    HWND hwnd;

    // Create the main window.
    hwnd = CreateWindow("OpenInputTest",
                        "OpenInput Win32 test window",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        (HWND)NULL,
                        (HMENU)NULL,
                        inst,
                        (LPVOID)NULL);

    // Store globals
    instance = inst;
    window = hwnd;

    // Show window, send update or bail
    if(!hwnd) {
        return FALSE;
    }
    ShowWindow(hwnd, showcmd);
    UpdateWindow(hwnd);

    return TRUE;
}

/* ******************************************************************** */

// Application entry point.
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev,
                   LPSTR cmdline, int showcmd) {
    char csw[100];
    int e;

    fprintf(stderr, "win32test: starting\n");

    // Init class and window
    if(!init_class(inst)) {
        fprintf(stderr, "win32test: class initialization error\n");
        return FALSE;
    }
    if(!init_window(inst, showcmd)) {
        fprintf(stderr, "win32test: window creation error\n");
        return FALSE;
    }

    // Init OI
    sprintf(csw, "c:%u s:%u w:%u", 0, 0, (unsigned int)window);
    printf("win32test: init parameters '%s'\n", csw);
    e = oi_init(csw, 0);
    fprintf(stderr, "win32test: oi_init, code %i\n", e);

    // Perform the test
    platform_test();

    // Close OI
    e = oi_close();
    fprintf(stderr, "win32test: oi_close, code %i\n", e);

    // Done
    fprintf(stderr, "win32test: finished\n");
    return 0;
}

/* ******************************************************************** */
