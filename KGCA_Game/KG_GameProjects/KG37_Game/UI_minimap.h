#pragma once
#include "UI_obj.h"
#include "KG_DxRT.h"
#include "KG_ShapeBox.h"

class KG_Camera;
namespace CBY
{
	class CBY_FPSCharacter;
	class CBY_EnemyRobot;
	class CBY_EnemySpider;
}
namespace KYS
{
	
	class UI_minimap : public UI_obj
	{
	public:
		UI_minimap();
		virtual ~UI_minimap();
	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
	public:

		virtual bool Init() override;
		virtual bool Frame()	override;
		virtual bool Render(KG_Camera* camera, ID3D11Device* _device, ID3D11DeviceContext* _context) override;
	public:
		void SetDeviceAndConntext(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			_device = device; _context = context;
		}

		void SetCamera(KG_Camera* camera) { _camera = camera; }
		void SetRecoverViewPort(D3D11_VIEWPORT* view) { _recoverViewPort = view; }
		void SetObj(std::shared_ptr<KG_ShapeBox> obj) { _boxObj = obj; }
		void SetViewOffset(float value) { _minimapViewOffset = value; }
		float GetViewOffset() { return _minimapViewOffset; }

	protected:
		KG_DxRT _renderTarget;
		D3D11_VIEWPORT _miniMapViewPort;
		D3D11_VIEWPORT* _recoverViewPort;
		KG_Camera*	_camera;
		D3DXMATRIX _matTopView;
		D3DXMATRIX _matTopProj;
		ID3D11Device*	_device;
		ID3D11DeviceContext* _context;
		//testDrawObj
		std::shared_ptr<KG_ShapeBox> _boxObj;
		std::shared_ptr<CBY::CBY_FPSCharacter> m_Character;
		std::vector<std::shared_ptr<CBY::CBY_EnemyRobot>> m_EnemyList;
		std::vector<std::shared_ptr<CBY::CBY_EnemySpider>> m_EnemySpiderList;

		bool _minimapAlpha;
		float _minimapViewOffset;
	};

}

