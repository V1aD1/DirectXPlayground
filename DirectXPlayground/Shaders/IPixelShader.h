#pragma once

#include "IShader.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

struct IPixelShader : public IShader {
	ComPtr<ID3D11PixelShader> m_directXObjPS;
};