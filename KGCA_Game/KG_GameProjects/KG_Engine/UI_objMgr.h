#pragma once
#include <map>
#include <memory>
#include "KG_Camera.h"
#include "UI_obj.h"
namespace KYS
{
	class UI_obj;
	class UI_objMgr
	{
	private:
		UI_objMgr();
	public:
		virtual ~UI_objMgr();

	public:
		static UI_objMgr* getInstance()
		{
			static UI_objMgr mgr;
			return &mgr;
		}
	public:
		bool Init();
		bool Frame();
		bool Render(KG_Camera* camera);
		bool Release();

	public:
		void BossUi();
		void add(std::shared_ptr<UI_obj > obj);
		std::shared_ptr<UI_obj> find(UI_TYPE type);
		std::shared_ptr<UI_obj> find(std::shared_ptr<UI_obj> obj);
		void ClearUiList();
	public:
		std::vector<std::shared_ptr<UI_obj>> getUiList() { return _uiList; }

		void setDevice(ID3D11Device* device) { _device = device; }
		void setContext(ID3D11DeviceContext* context) { _context = context; }
		void SetCamera(KG_Camera* camera) { _camera = camera; }

	private:
		std::vector<std::shared_ptr<UI_obj>> _uiList;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		KG_Camera* _camera;
	};

#define UI_MGR KYS::UI_objMgr::getInstance()
}