#pragma once

#include "Globals.h"
#include "IVertexShader.h"
#include "IPixelShader.h"

struct ShinyMatVS : public IVertexShader {
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() override;
};

struct ShinyMatPS : public IPixelShader {
};