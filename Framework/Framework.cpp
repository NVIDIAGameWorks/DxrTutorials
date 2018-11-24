/***************************************************************************
# Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
#include "Framework.h"
#include <locale>
#include <codecvt>

namespace
{
    HWND gWinHandle = nullptr;

    static LRESULT CALLBACK msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    HWND createWindow(const std::string& winTitle, uint32_t& width, uint32_t& height)
    {
        const WCHAR* className = L"DxrTutorialWindowClass";
        DWORD winStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

        // Load the icon
        HANDLE icon = LoadImageA(nullptr, _PROJECT_DIR_ "\\nvidia.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

        // Register the window class
        WNDCLASS wc = {};
        wc.lpfnWndProc = msgProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = className;
        wc.hIcon = (HICON)icon;

        if (RegisterClass(&wc) == 0)
        {
            msgBox("RegisterClass() failed");
            return nullptr;
        }

        // Window size we have is for client area, calculate actual window size
        RECT r{ 0, 0, (LONG)width, (LONG)height };
        AdjustWindowRect(&r, winStyle, false);

        int windowWidth = r.right - r.left;
        int windowHeight = r.bottom - r.top;

        // create the window
        std::wstring wTitle = string_2_wstring(winTitle);
        HWND hWnd = CreateWindowEx(0, className, wTitle.c_str(), winStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr, nullptr, wc.hInstance, nullptr);
        if (hWnd == nullptr)
        {
            msgBox("CreateWindowEx() failed");
            return nullptr;
        }

        return hWnd;
    }

    void msgLoop(Tutorial& tutorial)
    {
        MSG msg;
        while (1)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT) break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                tutorial.onFrameRender();
            }
        }
    }
};

std::wstring string_2_wstring(const std::string& s)
{
    std::wstring_convert<std::codecvt_utf8<WCHAR>> cvt;
    std::wstring ws = cvt.from_bytes(s);
    return ws;
}

std::string wstring_2_string(const std::wstring& ws)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    std::string s = cvt.to_bytes(ws);
    return s;
}

void msgBox(const std::string& msg)
{
    MessageBoxA(gWinHandle, msg.c_str(), "Error", MB_OK);
}

void d3dTraceHR(const std::string& msg, HRESULT hr)
{
    char hr_msg[512];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, 0, hr_msg, ARRAYSIZE(hr_msg), nullptr);

    std::string error_msg = msg + ".\nError! " + hr_msg;
    msgBox(error_msg);
}

void Framework::run(Tutorial& tutorial, const std::string& winTitle, uint32_t width, uint32_t height)
{
    gWinHandle = createWindow(winTitle, width, height);

    // Calculate the client-rect area
    RECT r;
    GetClientRect(gWinHandle, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;

    // Call onLoad()
    tutorial.onLoad(gWinHandle, width, height);
    
    // Show the window
    ShowWindow(gWinHandle, SW_SHOWNORMAL);

    // Start the msgLoop()
    msgLoop(tutorial);

    // Cleanup
    tutorial.onShutdown();
    DestroyWindow(gWinHandle);
}