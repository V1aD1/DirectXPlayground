#include "pch.h"
#include "ShinyMatShader.h"
#include "ConstantBuffers.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

std::vector<D3D11_INPUT_ELEMENT_DESC> ShinyMatVS::GetInputLayout() {
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		// 5th param specifies on which byte the new piece of info starts
		// so position starts on byte 0, next on byte 12 etc
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	return ied;
}