#include <iostream>
#include <Windows.h>
#include "Hooks.hpp"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#include <DirectX/d3d9.h>
#include <Detours/detours.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "detours.lib")

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    if (!isInitialized) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        D3DDEVICE_CREATION_PARAMETERS pParameters;
        pDevice->GetCreationParameters(&pParameters);
        hWindow = pParameters.hFocusWindow;

        if (hWindow) {
            SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGui_ImplWin32_Init(hWindow);
            ImGui_ImplDX9_Init(pDevice);
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
    return oEndScene(pDevice);
}

HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkSetTexture(IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture) {
    return oSetTexture(pDevice, Stage, pTexture);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    pDevice->Reset(pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return oReset(pDevice, pPresentationParameters);
}

HRESULT APIENTRY hkSetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) {
    return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkSetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9* pDecl) {
    return oSetVertexDeclaration(pDevice, pDecl);
}

HRESULT APIENTRY hkSetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount) {
    return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkSetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9* pShader) {
    return oSetVertexShader(pDevice, pShader);
}

HRESULT APIENTRY hkSetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9* pShader) {
    return oSetPixelShader(pDevice, pShader);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (isInitialized && isMenuVisible && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return TRUE;
    }

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        TerminateProcess(hProcess, 0);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
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

    IDirect3D9* (WINAPI * Direct3DCreate9Detoured)(UINT) = (IDirect3D9 * (WINAPI*)(UINT))DetourFindFunction("d3d9.dll", "Direct3DCreate9");
    if (!Direct3DCreate9Detoured) {
        return FALSE;
    }

    IDirect3D9* pDirect3D9 = Direct3DCreate9Detoured(D3D_SDK_VERSION);
    if (!pDirect3D9) {
        return FALSE;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice9* pDevice = NULL;
    if (pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice) != D3D_OK) {
        pDirect3D9->Release();
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

    oEndScene = (EndScene)vTable[42];
    oPresent = (Present)vTable[17];
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)vTable[82];
    oDrawPrimitive = (DrawPrimitive)vTable[81];
    oSetTexture = (SetTexture)vTable[65];
    oReset = (Reset)vTable[16];
    oSetStreamSource = (SetStreamSource)vTable[100];
    oSetVertexDeclaration = (SetVertexDeclaration)vTable[87];
    oSetVertexShaderConstantF = (SetVertexShaderConstantF)vTable[94];
    oSetVertexShader = (SetVertexShader)vTable[92];
    oSetPixelShader = (SetPixelShader)vTable[107];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)oEndScene, (PBYTE)hkEndScene);
    DetourAttach(&(LPVOID&)oPresent, (PBYTE)hkPresent);
    DetourAttach(&(LPVOID&)oDrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
    DetourAttach(&(LPVOID&)oDrawPrimitive, (PBYTE)hkDrawPrimitive);
    DetourAttach(&(LPVOID&)oSetTexture, (PBYTE)hkSetTexture);
    DetourAttach(&(LPVOID&)oReset, (PBYTE)hkReset);
    DetourAttach(&(LPVOID&)oSetStreamSource, (PBYTE)hkSetStreamSource);
    DetourAttach(&(LPVOID&)oSetVertexDeclaration, (PBYTE)hkSetVertexDeclaration);
    DetourAttach(&(LPVOID&)oSetVertexShaderConstantF, (PBYTE)hkSetVertexShaderConstantF);
    DetourAttach(&(LPVOID&)oSetVertexShader, (PBYTE)hkSetVertexShader);
    DetourAttach(&(LPVOID&)oSetPixelShader, (PBYTE)hkSetPixelShader);
    DetourTransactionCommit();

    pDevice->Release();
    pDirect3D9->Release();
    CloseHandle(hThread);
    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(NULL, 0, initializeHook, NULL, 0, NULL);
        if (!hThread) {
            return FALSE;
        }
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        if (hThread) {
            CloseHandle(hThread);
            hThread = NULL;
        }

        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        break;
    }
    return TRUE;
}