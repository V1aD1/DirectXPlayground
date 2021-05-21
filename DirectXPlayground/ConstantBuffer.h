#pragma once

// used to mirror our constant buffer
struct CONSTANTBUFFER {

	// var names don't have to match with constant buffer BUT
	// variable size and order must match;

	// final transformation
	XMMATRIX matFinal{}; // bytes 0 - 15 
	XMMATRIX rotation{};
	//float padding1; // bytes 12 - 15 

	XMVECTOR diffuseVector{};
	XMVECTOR diffuseColor{};
	XMVECTOR ambientColor{};
};