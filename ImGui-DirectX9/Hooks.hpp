#pragma once
#include <DirectX/d3d9.h>
#pragma comment(lib, "d3d9.lib")

HANDLE hProcess = GetCurrentProcess();
HANDLE hThread = NULL;
HWND hWindow = NULL;
bool isInitialized = false;
bool isMenuVisible = true;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void inputHandler();

#define HOOK(FuncName, ReturnType, ...) \
    typedef ReturnType(APIENTRY* FuncName##Func)(__VA_ARGS__); \
    FuncName##Func o##FuncName = nullptr;

typedef HRESULT(APIENTRY* EndScene)(IDirect3DDevice9*);
typedef HRESULT(APIENTRY* Present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
typedef HRESULT(APIENTRY* DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
typedef HRESULT(APIENTRY* SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
typedef HRESULT(APIENTRY* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef HRESULT(APIENTRY* SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
typedef HRESULT(APIENTRY* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
typedef HRESULT(APIENTRY* SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
typedef HRESULT(APIENTRY* SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
typedef HRESULT(APIENTRY* SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);

HOOK(EndScene, HRESULT, IDirect3DDevice9*)
HOOK(Present, HRESULT, IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)
HOOK(DrawIndexedPrimitive, HRESULT, IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT)
HOOK(DrawPrimitive, HRESULT, IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT)
HOOK(SetTexture, HRESULT, IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*)
HOOK(Reset, HRESULT, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)
HOOK(SetStreamSource, HRESULT, IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT)
HOOK(SetVertexDeclaration, HRESULT, IDirect3DDevice9*, IDirect3DVertexDeclaration9*)
HOOK(SetVertexShaderConstantF, HRESULT, IDirect3DDevice9*, UINT, const float*, UINT)
HOOK(SetVertexShader, HRESULT, IDirect3DDevice9*, IDirect3DVertexShader9*)
HOOK(SetPixelShader, HRESULT, IDirect3DDevice9*, IDirect3DPixelShader9*)