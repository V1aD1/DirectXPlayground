#pragma once

#include "Globals.h"
#include "IShader.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

struct IVertexShader : public IShader {
	Shaders m_key;
	Array<byte>^ m_vsFile;

	virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() = 0;
};