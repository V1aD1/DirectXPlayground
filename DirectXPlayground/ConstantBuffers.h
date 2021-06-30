#pragma once

// REMEMBER constant buffers MUST be multiples of 16 bytes!! 
// If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored

// used to mirror our constant buffer
struct CONSTANTBUFFER {

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

struct SHINYMATCONSTBUFF {
	XMMATRIX matFinal{}; // 16 byte alligned
	XMMATRIX rotation{}; // 16 byte alligned

	XMVECTOR color{}; // 16 byte alligned
};