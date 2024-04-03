#include <iostream>
#include <Windows.h>
#include "Hooks.hpp"
#include <DirectX/d3d9.h>
#include <Detours/detours.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "detours.lib")

HANDLE hProcess = GetCurrentProcess();
HANDLE hThread = NULL;
HWND hWindow = NULL;
bool isInitialized = false;
bool isMenuVisible = true;

HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
    if (!isInitialized) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        D3DDEVICE_CREATION_PARAMETERS pParameters;
        pDevice->GetCreationParameters(&pParameters);
        hWindow = pParameters.hFocusWindow;

        if (hWindow) {
            ImGui_ImplWin32_Init(hWindow);
            ImGui_ImplDX9_Init(pDevice);
            ImGui_ImplDX9_CreateDeviceObjects();
            SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
            isInitialized = true;
        }
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        isMenuVisible = !isMenuVisible;
        ImGui::GetIO().MouseDrawCursor = isMenuVisible;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (isMenuVisible) {
        inputHandler();
        ImGui::ShowDemoWindow();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    return oEndScene(pDevice);
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    pDevice->Reset(pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return oReset(pDevice, pPresentationParameters);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (isInitialized && isMenuVisible) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return TRUE;
    }
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        TerminateProcess(hProcess, 0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void inputHandler() {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    for (int i = 1; i < 5; i++) {
        io.MouseDown[i] = false;
    }
}

DWORD WINAPI initializeHook(LPVOID lpParam) {
    HMODULE hModuleD3D9 = GetModuleHandle("d3d9.dll");
    if (!hModuleD3D9) {
        return FALSE;
    }

    IDirect3DDevice9* pDevice = nullptr;
    IDirect3D9* pDirect = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pDirect) {
        return FALSE;
    }

    D3DPRESENT_PARAMETERS gD3DPresentParams;
    ZeroMemory(&gD3DPresentParams, sizeof(gD3DPresentParams));
    gD3DPresentParams.Windowed = TRUE;
    gD3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    gD3DPresentParams.BackBufferFormat = D3DFMT_UNKNOWN;
    gD3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (pDirect->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &gD3DPresentParams, &pDevice) != D3D_OK) {
        return FALSE;
    }

    DWORD_PTR* vTable = NULL;
#ifdef _WIN64
    DWORD64* dvTable64 = reinterpret_cast<DWORD64*>(pDevice);
    vTable = reinterpret_cast<DWORD_PTR*>(dvTable64[0]);
#else
    DWORD* dvTable32 = reinterpret_cast<DWORD*>(pDevice);
    vTable = reinterpret_cast<DWORD_PTR*>(dvTable32[0]);
#endif

    oPresent = (Present)vTable[17];
    oEndScene = (EndScene)vTable[42];   
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)vTable[82];
    oDrawPrimitive = (DrawPrimitive)vTable[81];
    oReset = (Reset)vTable[16];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)oPresent, (PBYTE)hkPresent);
    DetourAttach(&(LPVOID&)oEndScene, (PBYTE)hkEndScene);
    DetourAttach(&(LPVOID&)oDrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
    DetourAttach(&(LPVOID&)oDrawPrimitive, (PBYTE)hkDrawPrimitive);
    DetourAttach(&(LPVOID&)oReset, (PBYTE)hkReset);
    DetourTransactionCommit();

    clearVariable(pDirect);
    clearVariable(pDevice);
    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        if (!(hThread = CreateThread(NULL, 0, initializeHook, NULL, 0, NULL))){
            return FALSE;
        }
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        if (isInitialized) {
            ImGui_ImplDX9_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
        break;
    }
    return TRUE;
}