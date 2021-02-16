#pragma once

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

class CGame {
public:
	// device interface
	ComPtr<ID3D11Device1> m_dev;

	//device context interface
	ComPtr<ID3D11DeviceContext1> m_devCon;

	// swap chain interface
	ComPtr<IDXGISwapChain1> m_swapChain;

	void Initialize();
	void Update();
	void Render();
};