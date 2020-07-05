#pragma once
#include "KG_Std.h"
#include "KG_Camera.h"

namespace KYS
{

	class Scene
	{	
	public:
		Scene();
		virtual ~Scene();
	public:
		virtual bool Init()		= 0;
		virtual bool Frame()	= 0;
		virtual bool Render()	= 0;
		virtual bool Release()	= 0;
	
	public:
		void setDevice(ID3D11Device* device) { _device = device; };
		void setDeviceContext(ID3D11DeviceContext* context) { _deviceContext = context; };
		void setSwapChain(IDXGISwapChain* swapChain) { _swapChain = swapChain; };
		void setMainCamera(KG_Camera** camera) { _camera = camera;  m_pMainCamera = *camera; };
		void setViewPort(D3D11_VIEWPORT* view) { m_ViewPort = view; }
		
		void SetActiveNextScene(bool able) { _activeNextScene = able; }
		bool GetActiveNextScene() { return _activeNextScene; }
	protected:
		ID3D11Device*			_device;
		ID3D11DeviceContext*	_deviceContext;
		IDXGISwapChain*			_swapChain;
		KG_Camera**				_camera;
		KG_Camera*				m_pMainCamera;
		D3D11_VIEWPORT*			m_ViewPort;
		bool					_activeNextScene;
	};
}