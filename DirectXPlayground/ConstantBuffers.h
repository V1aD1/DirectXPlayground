#pragma once

// REMEMBER constant buffers MUST be multiples of 16 bytes!! 
// If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored

// used to mirror our vs constant buffer
struct TEXTURECONSTBUFF_VS {

	// var names don't have to match with constant buffer BUT
	// variable size and order must match;

	// final transformation
	XMMATRIX matFinal{}; // bytes 0 - 63
	XMMATRIX rotation{}; // bytes 62 - 127
	//float padding1;

	XMVECTOR diffuseVector{};
	XMVECTOR diffuseColor{};
	XMVECTOR ambientColor{};
};

struct SHINYMATCONSTBUFF_VS {
	XMMATRIX matFinal{}; // 16 byte alligned
	XMMATRIX rotation{}; // 16 byte alligned
	XMVECTOR color{}; // 16 byte alligned
	XMFLOAT3 camPos{}; // NOT 16 byte alligned!
	float padding1; // now we're good
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
};

struct SHINYMATCONSTBUFF_PS {
	XMVECTOR ambientColor{};
	XMVECTOR diffuseColor{};
	XMVECTOR specularColor;
	XMVECTOR lightDir{};
	float specularPower;
	XMFLOAT3 padding1;
};