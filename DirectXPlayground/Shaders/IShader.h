#pragma once

#include <vector>

#include "Globals.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

struct IShader {
	ShaderKeys m_key;
	Array<byte>^ m_shaderFile;
	int m_constBufferSize;

	D3D11_BUFFER_DESC GetConstBufferDesc();
};

