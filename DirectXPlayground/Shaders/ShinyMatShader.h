#pragma once

#include "Globals.h"
#include "IVertexShader.h"

struct ShinyMatVS : public IVertexShader {
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() override;
	virtual const void* GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) override;
};
