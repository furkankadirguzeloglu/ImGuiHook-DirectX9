#pragma once
#include <DirectX/d3d9.h>
#pragma comment(lib, "d3d9.lib")

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

/*D3D9 Methods Table :
[0] QueryInterface
[1] AddRef
[2] Release
[3] TestCooperativeLevel
[4] GetAvailableTextureMem
[5] EvictManagedResources
[6] GetDirect3D
[7] GetDeviceCaps
[8] GetDisplayMode
[9] GetCreationParameters
[10] SetCursorProperties
[11] SetCursorPosition
[12] ShowCursor
[13] CreateAdditionalSwapChain
[14] GetSwapChain
[15] GetNumberOfSwapChains
[16] Reset
[17] Present
[18] GetBackBuffer
[19] GetRasterStatus
[20] SetDialogBoxMode
[21] SetGammaRamp
[22] GetGammaRamp
[23] CreateTexture
[24] CreateVolumeTexture
[25] CreateCubeTexture
[26] CreateVertexBuffer
[27] CreateIndexBuffer
[28] CreateRenderTarget
[29] CreateDepthStencilSurface
[30] UpdateSurface
[31] UpdateTexture
[32] GetRenderTargetData
[33] GetFrontBufferData
[34] StretchRect
[35] ColorFill
[36] CreateOffscreenPlainSurface
[37] SetRenderTarget
[38] GetRenderTarget
[39] SetDepthStencilSurface
[40] GetDepthStencilSurface
[41] BeginScene
[42] EndScene
[43] Clear
[44] SetTransform
[45] GetTransform
[46] MultiplyTransform
[47] SetViewport
[48] GetViewport
[49] SetMaterial
[50] GetMaterial
[51] SetLight
[52] GetLight
[53] LightEnable
[54] GetLightEnable
[55] SetClipPlane
[56] GetClipPlane
[57] SetRenderState
[58] GetRenderState
[59] CreateStateBlock
[60] BeginStateBlock
[61] EndStateBlock
[62] SetClipStatus
[63] GetClipStatus
[64] GetTexture
[65] SetTexture
[66] GetTextureStageState
[67] SetTextureStageState
[68] GetSamplerState
[69] SetSamplerState
[70] ValidateDevice
[71] SetPaletteEntries
[72] GetPaletteEntries
[73] SetCurrentTexturePalette
[74] GetCurrentTexturePalette
[75] SetScissorRect
[76] GetScissorRect
[77] SetSoftwareVertexProcessing
[78] GetSoftwareVertexProcessing
[79] SetNPatchMode
[80] GetNPatchMode
[81] DrawPrimitive
[82] DrawIndexedPrimitive
[83] DrawPrimitiveUP
[84] DrawIndexedPrimitiveUP
[85] ProcessVertices
[86] CreateVertexDeclaration
[87] SetVertexDeclaration
[88] GetVertexDeclaration
[89] SetFVF
[90] GetFVF
[91] CreateVertexShader
[92] SetVertexShader
[93] GetVertexShader
[94] SetVertexShaderConstantF
[95] GetVertexShaderConstantF
[96] SetVertexShaderConstantI
[97] GetVertexShaderConstantI
[98] SetVertexShaderConstantB
[99] GetVertexShaderConstantB
[100] SetStreamSource
[101] GetStreamSource
[102] SetStreamSourceFreq
[103] GetStreamSourceFreq
[104] SetIndices
[105] GetIndices
[106] CreatePixelShader
[107] SetPixelShader
[108] GetPixelShader
[109] SetPixelShaderConstantF
[110] GetPixelShaderConstantF
[111] SetPixelShaderConstantI
[112] GetPixelShaderConstantI
[113] SetPixelShaderConstantB
[114] GetPixelShaderConstantB
[115] DrawRectPatch
[116] DrawTriPatch
[117] DeletePatch
[118] CreateQuery*/