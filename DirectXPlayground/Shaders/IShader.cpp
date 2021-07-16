#include "pch.h"
#include "IShader.h"

D3D11_BUFFER_DESC IShader::GetConstBufferDesc() {
	D3D11_BUFFER_DESC vsbd = { 0 };
	vsbd.Usage = D3D11_USAGE_DEFAULT;

	// constant buffers MUST be multiples of 16 bytes. If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored
	vsbd.ByteWidth = m_constBufferSize;
	vsbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	return vsbd;
}
