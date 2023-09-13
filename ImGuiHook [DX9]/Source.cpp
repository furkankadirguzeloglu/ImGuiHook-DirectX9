#include <windows.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "Hook.h"

bool showMenu = true;
bool initImGui = false;

void InputHandler() {
    for (int i = 0; i < 5; i++) {
        ImGui::GetIO().MouseDown[i] = false;
    }

    int Button = -1;
    if (GetAsyncKeyState(VK_LBUTTON)) {
        Button = 0;
    }

    if (Button != -1) {
        ImGui::GetIO().MouseDown[Button] = true;
    }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (showMenu && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    return CallWindowProc(Process.TargetWndProc, hWnd, msg, wParam, lParam);
}

HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice) {
    if (pDevice == NULL)
        return oEndScene(pDevice);

    if (!initImGui) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        D3DDEVICE_CREATION_PARAMETERS pParameters;
        pDevice->GetCreationParameters(&pParameters);
        Process.TargetHwnd = pParameters.hFocusWindow;

        if (Process.TargetHwnd != NULL) {
            Process.TargetWndProc = (WNDPROC)SetWindowLongPtr(Process.TargetHwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGui_ImplWin32_Init(Process.TargetHwnd);
            ImGui_ImplDX9_Init(pDevice);
            ImGui::GetIO().ImeWindowHandle = Process.TargetHwnd;
            initImGui = true;
        }   
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1) 
        showMenu = !showMenu;

    ImGui::GetIO().MouseDrawCursor = showMenu;
    if (showMenu == true) {
        InputHandler();
        ImGui::ShowDemoWindow();
    }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return oEndScene(pDevice);
}

HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
    if (pDevice == NULL)
        return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    if (pDevice == NULL)
        return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

    return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    if (pDevice == NULL)
        return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);

	return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkSetTexture(IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture) {
    if (pDevice == NULL)
        return oSetTexture(pDevice, Stage, pTexture);

	return oSetTexture(pDevice, Stage, pTexture);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    if (pDevice == NULL)
        return oReset(pDevice, pPresentationParameters);

    D3DDEVICE_CREATION_PARAMETERS pParameters{ 0 };
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto ResetReturn = oReset(pDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return ResetReturn;
}

HRESULT APIENTRY hkSetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) {
    if (pDevice == NULL)
        return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkSetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9* pDecl) {
    if (pDevice == NULL)
        return oSetVertexDeclaration(pDevice, pDecl);

	return oSetVertexDeclaration(pDevice, pDecl);
}

HRESULT APIENTRY hkSetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount) {
    if (pDevice == NULL)
        return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);

	return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkSetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9* pShader) {
    if (pDevice == NULL)
        return oSetVertexShader(pDevice, pShader);

	return oSetVertexShader(pDevice, pShader);
}

HRESULT APIENTRY hkSetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9* pShader) {
    if (pDevice == NULL)
        return oSetPixelShader(pDevice, pShader);

	return oSetPixelShader(pDevice, pShader);
}

DWORD WINAPI InitHook(LPVOID lpParameter) {
    if (checktDirectXVersion(DirectXVersion.D3D9) == false) {
        return FALSE;
    }

    HWND WindowHwnd = CreateWindow("BUTTON", "DX", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Process.hModule, NULL);
    if (WindowHwnd == NULL) {
        return FALSE;
    }

    IDirect3D9* IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (IDirect3D9 == NULL) {
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    D3DPRESENT_PARAMETERS pParameters;
    ZeroMemory(&pParameters, sizeof(pParameters));
    pParameters.Windowed = TRUE;
    pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pParameters.hDeviceWindow = WindowHwnd;
    pParameters.BackBufferFormat = D3DFMT_UNKNOWN;

    IDirect3DDevice9* pDevice;
    if (IDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pParameters, &pDevice) != D3D_OK) {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    if (pDevice == NULL) {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

#if defined _M_X64
    DWORD64* DVTable = (DWORD64*)pDevice;
    DVTable = (DWORD64*)DVTable[0];
#elif defined _M_IX86
    DWORD* DVTable = (DWORD*)pDevice;
    DVTable = (DWORD*)DVTable[0];
#endif

    oEndScene = (EndScene)DVTable[42];
    oPresent = (Present)DVTable[17];
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)DVTable[82];
    oDrawPrimitive = (DrawPrimitive)DVTable[81];
    oSetTexture = (SetTexture)DVTable[65];
    oReset = (Reset)DVTable[16];
    oSetStreamSource = (SetStreamSource)DVTable[100];
    oSetVertexDeclaration = (SetVertexDeclaration)DVTable[87];
    oSetVertexShaderConstantF = (SetVertexShaderConstantF)DVTable[94];
    oSetVertexShader = (SetVertexShader)DVTable[92];
    oSetPixelShader = (SetPixelShader)DVTable[107];

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
    IDirect3D9->Release();
    DestroyWindow(WindowHwnd);
    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        Process.hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, InitHook, 0, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}