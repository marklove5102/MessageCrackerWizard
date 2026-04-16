/*****************************************************************

MIT License

This file is part of Message Cracker Wizard

Copyright (c) 2003, 2017, 2018, 2024 Hernán Di Pietro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

***************************************************************************/

#include "stdafx.h"
#include "MsgCrackWizard.h"

void LoadConfig(MCWCONFIG& mcw)
{
    HKEY hk;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, 
        L"Software\\MessageCrackerWizard", NULL, NULL, NULL, KEY_READ, NULL, &hk, NULL) == ERROR_SUCCESS)
    {
        mcw.rcWindow = { -1,-1,-1,-1 };
        mcw.bDarkMode = false;
        mcw.bStayOnTop = false;
        mcw.windowAlpha = ALPHA_SOLID;

        auto ReadDword = [hk](const wchar_t* valueName, DWORD& outValue) -> bool
        {
            DWORD cbData = sizeof(DWORD);
            DWORD valueType = 0;
            return RegQueryValueEx(hk, valueName, NULL, &valueType, (BYTE*)&outValue, &cbData) == ERROR_SUCCESS &&
                valueType == REG_DWORD &&
                cbData == sizeof(DWORD);
        };

        DWORD dwValue = 0;
        if (ReadDword(L"wRect.Top", dwValue))
            mcw.rcWindow.top = (LONG)dwValue;
        if (ReadDword(L"wRect.Left", dwValue))
            mcw.rcWindow.left = (LONG)dwValue;
        if (ReadDword(L"wRect.Right", dwValue))
            mcw.rcWindow.right = (LONG)dwValue;
        if (ReadDword(L"wRect.Bottom", dwValue))
            mcw.rcWindow.bottom = (LONG)dwValue;
        if (ReadDword(L"DarkMode", dwValue))
            mcw.bDarkMode = dwValue != 0;
        if (ReadDword(L"StayOnTop", dwValue))
            mcw.bStayOnTop = dwValue != 0;
        if (ReadDword(L"WindowAlpha", dwValue))
            mcw.windowAlpha = (int)dwValue;

        RegCloseKey(hk);
    }
    else
    {
        MessageBox(NULL, L"Cannot load or create MessageCrackerWizard registry entries", 
            L"Message Cracker Wizard", MB_OK | MB_SETFOREGROUND | MB_ICONWARNING);
    }
}

void SaveConfig(const MCWCONFIG& mcw)
{
    HKEY hk;
    if (RegCreateKeyEx(HKEY_CURRENT_USER,
        L"Software\\MessageCrackerWizard", NULL, NULL, NULL, KEY_WRITE, NULL, &hk, NULL) == ERROR_SUCCESS)
    {
        DWORD dwDarkMode = mcw.bDarkMode ? 1 :0;
        DWORD dwStayOnTop = mcw.bStayOnTop ? 1 : 0;

        RegSetValueEx(hk, L"wRect.Top", NULL, REG_DWORD, (BYTE*)&mcw.rcWindow.top, sizeof(DWORD));
        RegSetValueEx(hk, L"wRect.Left", NULL, REG_DWORD, (BYTE*)&mcw.rcWindow.left, sizeof(DWORD));
        RegSetValueEx(hk, L"wRect.Right", NULL, REG_DWORD, (BYTE*)&mcw.rcWindow.right, sizeof(DWORD));
        RegSetValueEx(hk, L"wRect.Bottom", NULL, REG_DWORD, (BYTE*)&mcw.rcWindow.bottom, sizeof(DWORD));
        RegSetValueEx(hk, L"DarkMode", NULL, REG_DWORD, (BYTE*)&dwDarkMode, sizeof(DWORD));
        RegSetValueEx(hk, L"StayOnTop", NULL, REG_DWORD, (BYTE*)&dwStayOnTop, sizeof(DWORD));
        RegSetValueEx(hk, L"WindowAlpha", NULL, REG_DWORD, (BYTE*)&mcw.windowAlpha, sizeof(DWORD));

        RegCloseKey(hk);
    }
    else
    {
        MessageBox(NULL, L"Cannot load or create MessageCrackerWizard registry entries",
            L"Message Cracker Wizard", MB_OK | MB_SETFOREGROUND | MB_ICONWARNING);
    }
}