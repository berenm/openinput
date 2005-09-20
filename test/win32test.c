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

#include "openinput.h"
#include <windows.h>
#include <stdio.h>

/* ******************************************************************** */

// Global vars
HINSTANCE instance;

/* ******************************************************************** */

// Window procedure
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg,
                             WPARAM wparam, LPARAM lparam) {
    // We only want the close/destroy message
    if((msg == WM_CLOSE) || (msg == WM_DESTROY)) {
        printf("win32test: catched close/destroy\n");
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
    wcx.lpszMenuName =  "MainMenu";
    wcx.lpszClassName = "MainWClass";
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

    instance = inst;

    // Create the main window.
    hwnd = CreateWindow("MainWClass",
                        "Sample",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        (HWND)NULL,
                        (HMENU)NULL,
                        inst,
                        (LPVOID)NULL);

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
int WINAPI WinMain(HINSTANCE inst, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int showcmd) {
    MSG msg;
    BOOL res;

    printf("*** win32test starting\n");

    // Init class and window
    if(!init_class(inst)) {
        printf("*** class initialization error\n");
        return FALSE;
    }
    if(!init_window(inst, showcmd)) {
        printf("*** window creation error\n");
        return FALSE;
    }

    // Message loop
    while(((res = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0) && (res != -1)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("*** win32test finished\n");
    return msg.wParam;
}

/* ******************************************************************** */
