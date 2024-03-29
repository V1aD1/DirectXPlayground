#pragma once

#include "IShader.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

struct IVertexShader : public IShader {
	ComPtr<ID3D11VertexShader> m_directXObjVS;
	virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() = 0;
};