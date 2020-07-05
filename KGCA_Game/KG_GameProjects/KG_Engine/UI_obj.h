#pragma once
#include "KG_ShapeObject.h"

enum class UI_VALUE
{
	UI_VALUE_ADD_HP = 0,
	UI_VALUE_ADD_HPMAX,
	UI_VALUE_SET_HP,
	UI_VALUE_SET_HPMAX,
	UI_VALUE_SET_INIT_HP_HPMAX,
	UI_VALUE_ADD_LENGTH,
	UI_VALUE_ADD_LENGTHMAX,
	UI_VALUE_SET_LENGTH,
	UI_VALUE_SET_LENGTHMAX,
	UI_VALUE_SET_INIT_LENGTH_LENGTHMAX,
	UI_VALUE_ADD_AMMO,
	UI_VALUE_ADD_AMMOMAX,
	UI_VALUE_SET_AMMO,
	UI_VALUE_SET_AMMOMAX,
	UI_VALUE_SET_MAGAZINEMAX,
	UI_VALUE_SET_INIT_AMMO,

	UI_VALUE_SET_RECOVER_VIEWPORT,
	UI_VALUE_SET_MINIMAP_DRAW_DEST,
	UI_VALUE_SET_MINIMAP_DRAW_DEST_CHARACTER,
	UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY,
	UI_VALUE_SET_MINIMAP_DRAW_DEST_TESTOBJ,
	UI_VALUE_SET_MINIMAP_VIEW_OFFSET,
	UI_VALUE_ADD_MINIMAP_VIEW_OFFSET,
	UI_VALUE_SET_MINIMAP_ALPHA,
	UI_VALUE_SET_CAMERA

};

enum class UI_TYPE
{
	UI_TYPE_NONE = 0,
	UI_TYPE_HP_PANEL,
	UI_TYPE_WEAPON_PANEL,
	UI_TYPE_BOSS_HP_PANEL,
	UI_TYPE_HP_BAR,
	UI_TYPE_HP_BAR_INNER,
	UI_TYPE_HP_BAR_FRAME,
	UI_TYPE_BOSS_HP_BAR,
	UI_TYPE_BOSS_HP_BAR_INNER,
	UI_TYPE_BOSS_HP_BAR_FRAME,
	UI_TYPE_AMMO_FONT,
	UI_TYPE_MAXAMMO_FONT,
	UI_TYPE_CHARACTER_PORTRAIT,
	UI_TYPE_WEAPON_PORTRAIT,
	UI_TYPE_BOSS_PORTRAIT,
	UI_TYPE_MINIMAP,
	UI_TYPE_MINIMAP_FRAME,
	UI_TYPE_TITLE_PANEL,
	UI_TYPE_BTN_START,
	UI_TYPE_COUNT
};
class KG_Camera;
namespace KYS
{
	class UI_obj : public KG_ShapeObject
	{
	public:
		UI_obj();
		virtual ~UI_obj();
	public:

		virtual void UpdateData();
		//가변인자를 받아서  UI 
		virtual void SetValue(UI_VALUE type, int paramCount, ...);
		virtual void SetType(UI_TYPE type, UI_VALUE value, int paramCount, ...);
		//Ui 밑으로 UI추가 
		virtual UI_obj* AddUI(UI_obj* obj) { return nullptr; }
	public:

		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Render()	override;
		virtual bool Render(KG_Camera* camera, ID3D11Device* _device, ID3D11DeviceContext* _context);
		virtual bool Release()	override;
	public:
		void SetPos(D3DXVECTOR3 pos) { m_vPos = pos; }
		D3DXVECTOR3 GetPos() { return m_vPos; }

		virtual void SetScale(D3DXVECTOR3 scale) { _scale = scale; _scaleMax = scale; if (_scale <= 0) _scale = D3DXVECTOR3(0, 0, 0); }
		D3DXVECTOR3 GetScale() { return _scale; }

		void SetRotate(D3DXVECTOR3 rotate) { _rotate = rotate; }
		D3DXVECTOR3 GetRotate() { return _rotate; }
		
		void SetPanelParts(D3DXVECTOR2 parts[9]) { memcpy_s(_rtParts, sizeof(_rtParts), parts, sizeof(parts)); }
		D3DXVECTOR2* getPanel() { return _rtParts; }

		void SetParentMatWorld(D3DXMATRIX mat) { _parentMatWorld = mat; }
		D3DXMATRIX GetParentMatWorld() { return _parentMatWorld; }

		void SetMatWorld(D3DXMATRIX mat) { m_matWorld = mat; }
		D3DXMATRIX GetMatWorld() { return m_matWorld; }

		void SetUiType(UI_TYPE type) { _uiType = type; }
		UI_TYPE GetUiType() { return _uiType; }

	protected:
		D3DXVECTOR2	_rtParts[9];
		D3DXVECTOR3 _scale;
		D3DXVECTOR3 _scaleMax;
		D3DXVECTOR3 _rotate;
		D3DXMATRIX _parentMatWorld;
		UI_TYPE _uiType;
	};
}

