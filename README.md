# ImGuiHook-DirectX9
ImGuiHook is a project that enables creating a graphical interface by hooking DirectX functions and utilizing the ImGui library. It operates by injecting a DLL into the target DirectX application.

This project is for DirectX 9. You can visit my GitHub page for other DirectX versions.


## Hooks
* Present
* EndScene
* DrawIndexedPrimitive
* DrawPrimitive
* Reset

## D3D9 Methods Tables
* [0] QueryInterface
* [1] AddRef
* [2] Release
* [3] TestCooperativeLevel
* [4] GetAvailableTextureMem
* [5] EvictManagedResources
* [6] GetDirect3D
* [7] GetDeviceCaps
* [8] GetDisplayMode
* [9] GetCreationParameters
* [10] SetCursorProperties
* [11] SetCursorPosition
* [12] ShowCursor
* [13] CreateAdditionalSwapChain
* [14] GetSwapChain
* [15] GetNumberOfSwapChains
* [16] Reset
* [17] Present
* [18] GetBackBuffer
* [19] GetRasterStatus
* [20] SetDialogBoxMode
* [21] SetGammaRamp
* [22] GetGammaRamp
* [23] CreateTexture
* [24] CreateVolumeTexture
* [25] CreateCubeTexture
* [26] CreateVertexBuffer
* [27] CreateIndexBuffer
* [28] CreateRenderTarget
* [29] CreateDepthStencilSurface
* [30] UpdateSurface
* [31] UpdateTexture
* [32] GetRenderTargetData
* [33] GetFrontBufferData
* [34] StretchRect
* [35] ColorFill
* [36] CreateOffscreenPlainSurface
* [37] SetRenderTarget
* [38] GetRenderTarget
* [39] SetDepthStencilSurface
* [40] GetDepthStencilSurface
* [41] BeginScene
* [42] EndScene
* [43] Clear
* [44] SetTransform
* [45] GetTransform
* [46] MultiplyTransform
* [47] SetViewport
* [48] GetViewport
* [49] SetMaterial
* [50] GetMaterial
* [51] SetLight
* [52] GetLight
* [53] LightEnable
* [54] GetLightEnable
* [55] SetClipPlane
* [56] GetClipPlane
* [57] SetRenderState
* [58] GetRenderState
* [59] CreateStateBlock
* [60] BeginStateBlock
* [61] EndStateBlock
* [62] SetClipStatus
* [63] GetClipStatus
* [64] GetTexture
* [65] SetTexture
* [66] GetTextureStageState
* [67] SetTextureStageState
* [68] GetSamplerState
* [69] SetSamplerState
* [70] ValidateDevice
* [71] SetPaletteEntries
* [72] GetPaletteEntries
* [73] SetCurrentTexturePalette
* [74] GetCurrentTexturePalette
* [75] SetScissorRect
* [76] GetScissorRect
* [77] SetSoftwareVertexProcessing
* [78] GetSoftwareVertexProcessing
* [79] SetNPatchMode
* [80] GetNPatchMode
* [81] DrawPrimitive
* [82] DrawIndexedPrimitive
* [83] DrawPrimitiveUP
* [84] DrawIndexedPrimitiveUP
* [85] ProcessVertices
* [86] CreateVertexDeclaration
* [87] SetVertexDeclaration
* [88] GetVertexDeclaration
* [89] SetFVF
* [90] GetFVF
* [91] CreateVertexShader
* [92] SetVertexShader
* [93] GetVertexShader
* [94] SetVertexShaderConstantF
* [95] GetVertexShaderConstantF
* [96] SetVertexShaderConstantI
* [97] GetVertexShaderConstantI
* [98] SetVertexShaderConstantB
* [99] GetVertexShaderConstantB
* [100] SetStreamSource
* [101] GetStreamSource
* [102] SetStreamSourceFreq
* [103] GetStreamSourceFreq
* [104] SetIndices
* [105] GetIndices
* [106] CreatePixelShader
* [107] SetPixelShader
* [108] GetPixelShader
* [109] SetPixelShaderConstantF
* [110] GetPixelShaderConstantF
* [111] SetPixelShaderConstantI
* [112] GetPixelShaderConstantI
* [113] SetPixelShaderConstantB
* [114] GetPixelShaderConstantB
* [115] DrawRectPatch
* [116] DrawTriPatch
* [117] DeletePatch
* [118] CreateQuery

## Acknowledgments
* [ImGui v1.90.4](https://github.com/ocornut/imgui) - Dear ImGui: Bloat-free Graphical User interface for C++ with minimal dependencies
* [DirectX Software Development Kit](https://www.microsoft.com/en-us/download/details.aspx?id=6812) - This DirectX SDK release contains updates to tools, utilities, samples, documentation, and runtime debug files for x64 and x86 platforms.
* [Microsoft Detours](https://github.com/microsoft/Detours) - Microsoft Detours is an open source library for intercepting, monitoring and instrumenting binary functions on Microsoft Windows. It is developed by Microsoft and is most commonly used to intercept Win32 API calls within Windows applications.

## License
This project is licensed under the terms of the MIT license. See the [LICENSE](https://raw.githubusercontent.com/furkankadirguzeloglu/ImGuiHook-DirectX11/main/LICENSE) file for details.
