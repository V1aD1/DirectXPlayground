#include "pch.h"
#include "Game.h"

// initializes and prepares Direct3D for use
void CGame::Initialize(){
	// define temp pointers to a device and device context
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	// create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11);

	// convert the pointers from the directx 11 versions to the directx 11.1 versions
	dev11.As(&m_dev);
	devcon11.As(&m_devcon);
}

// performs updates to the state of the game
void CGame::Update(){}

// renders a single frame of 3D graphics
void CGame::Render(){}