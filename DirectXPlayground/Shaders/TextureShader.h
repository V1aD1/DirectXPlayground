#pragma once

#include <vector>

#include "Globals.h"
#include "IVertexShader.h"
#include "IPixelShader.h"

struct TextureVS : public IVertexShader {
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() override;
};

struct TexturePS : public IPixelShader {
};