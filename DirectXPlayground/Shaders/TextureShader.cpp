#include "pch.h"
#include "TextureShader.h"
#include "ConstantBuffers.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <streambuf>
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;


std::vector<D3D11_INPUT_ELEMENT_DESC> TextureVS::GetInputLayout() {
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		// 5th param specifies on which byte the new piece of info starts
		// so position starts on byte 0, next on byte 12 etc
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	return ied;
}

const void* TextureVS::GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) {
	CONSTANTBUFFER* constBufVals = new CONSTANTBUFFER();

	constBufVals->matFinal = matFinal;
	constBufVals->rotation = rot;
	constBufVals->ambientColor = XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f); // the higher the RGB values, the brighter the light
	constBufVals->diffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	constBufVals->diffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	return constBufVals;
}
