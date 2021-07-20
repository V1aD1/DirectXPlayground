#pragma once

#include <vector>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

struct IShader {
	ComPtr<ID3D11VertexShader> m_directXShaderObj;
	int m_constBufferSize;

	D3D11_BUFFER_DESC GetConstBufferDesc();
};

