#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <thread>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "user32.lib")

auto dllBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("UnityPlayer.dll"));
auto ptr_1 = reinterpret_cast<DWORD_PTR*>(dllBase + 0x019DD5C8);
auto ptr_2 = reinterpret_cast<DWORD_PTR*>(*ptr_1 + 0x128);
auto ptr_3 = reinterpret_cast<DWORD_PTR*>(*ptr_2 + 0x90);
auto ptr_4 = reinterpret_cast<DWORD_PTR*>(*ptr_3 + 0x10);
auto ptr_5 = reinterpret_cast<DWORD_PTR*>(*ptr_4 + 0x60);
auto ptr_6 = reinterpret_cast<DWORD_PTR*>(*ptr_5 + 0x68);
auto skills_pointer = reinterpret_cast<DWORD_PTR*>(*ptr_6 + 0x4C);
constexpr long refresh_delay = 2000;

bool isActive = true;

void BindConsole()
{
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
}

void do_work()
{
    BindConsole();
    // createWindow();
    while (true)
    {
        if (!isActive)
        {
            return;
        }

        try {
            std::cout << "DllBase: " << dllBase << std::endl;
            std::cout << "Quantidade de skills: " << *skills_pointer << std::endl;
            *skills_pointer = 0;
        } catch (...){
            std::cout << "Erro ao acessar a memoria" << std::endl;
        }
        
        // isActive = false;
        Sleep(refresh_delay);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)do_work, NULL, NULL, NULL);
            break;
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }

    return TRUE;
}

void createWindow()
{
    HWND hWnd = CreateWindowEx(0, (LPWSTR)"STATIC", (LPWSTR)"Janela", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, NULL, NULL);
    ShowWindow(hWnd, SW_SHOW);

    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &pDevice, nullptr,
                      &pContext);

    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 1, (LPWSTR)"Item de menu 1");
    AppendMenu(hMenu, MF_STRING, 2, (LPWSTR)"Item de menu 2");

    DrawMenuBar(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyMenu(hMenu);

    pContext->Release();
    pDevice->Release();
}
