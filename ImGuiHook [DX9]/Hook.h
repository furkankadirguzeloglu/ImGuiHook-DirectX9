#include <d3d9.h>
#include <d3dx9.h>
#include <detours.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

struct CurrentProcess {
	HMODULE hModule;
	HWND TargetHwnd;
	WNDPROC TargetWndProc;
}Process;

struct _DirectXVersion {
	int Unknown = 0;
	int D3D9 = 1;
	int D3D10 = 2;
	int D3D11 = 3;
	int D3D12 = 4;
}DirectXVersion;

bool checktDirectXVersion(int _DirectXVersion) {
	if (_DirectXVersion = DirectXVersion.D3D12) {
		if (GetModuleHandle("d3d12.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion = DirectXVersion.D3D11) {
		if (GetModuleHandle("d3d11.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion = DirectXVersion.D3D10) {
		if (GetModuleHandle("d3d10.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion = DirectXVersion.D3D9) {
		if (GetModuleHandle("d3d9.dll") != NULL) {
			return true;
		}
	}

	return false;
}

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
EndScene oEndScene;

typedef HRESULT(APIENTRY* Present) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
Present oPresent;

typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive oDrawIndexedPrimitive;

typedef HRESULT(APIENTRY* DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
DrawPrimitive oDrawPrimitive;

typedef HRESULT(APIENTRY* SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
SetTexture oSetTexture;

typedef HRESULT(APIENTRY* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset oReset;

typedef HRESULT(APIENTRY* SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource oSetStreamSource;

typedef HRESULT(APIENTRY* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration oSetVertexDeclaration;

typedef HRESULT(APIENTRY* SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF oSetVertexShaderConstantF;

typedef HRESULT(APIENTRY* SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
SetVertexShader oSetVertexShader;

typedef HRESULT(APIENTRY* SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);
SetPixelShader oSetPixelShader;