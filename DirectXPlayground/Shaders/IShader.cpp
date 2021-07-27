#include "pch.h"
#include "IShader.h"

D3D11_BUFFER_DESC IShader::GetConstBufferDesc() {
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;

	// constant buffers MUST be multiples of 16 bytes. If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored
	bd.ByteWidth = m_constBufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	return bd;
}
