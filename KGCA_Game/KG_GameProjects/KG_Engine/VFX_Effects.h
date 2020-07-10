#pragma once
#include "VFX_EffectObj.h"

enum class VFX_EFFECT_TYPE
{
	VFX_EFFECT_NONE = 0,
	VFX_EFFECT_GUN1_SHOT,
	VFX_EFFECT_CHARACTER_BLOOD_IMPACT,
	VFX_EFFECT_SCREEN_BLOOD_IMPACT,
	VFX_EFFECT_BOSS_FIRE,
	VFX_EFFECT_BOSS_FIREBALL,
	VFX_EFFECT_BOSS_GROUND,
	VFX_EFFECT_COUNT
};

class KG_Camera;
namespace KYS
{
	class VFX_Effects
	{
	public: // construct

		VFX_Effects();
		VFX_Effects(VFX_Effects* obj);
		virtual ~VFX_Effects();
		VFX_Effects& operator=(KYS::VFX_Effects& _Right);
	public: // interface func and base func

		bool Init();
		bool Frame();
		bool Render(KG_Camera* camera, ID3D11Device* _device, ID3D11DeviceContext* _context);
		bool Release();

	public: //virtual

	public: //own func

		int Add(std::shared_ptr<VFX_EffectObj> obj);
		std::shared_ptr<VFX_EffectObj> GetEffectObj(int index);
		int GetEffectObj(std::shared_ptr<VFX_EffectObj> obj);

		void Execute(D3DXMATRIX* parentMat, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		void Shutdown();
		void UpdateState();
		void UpdateMatrix();
		void ApplyToChildren(D3DXMATRIX mat);
		void ResetState();

	public: //getter, setter

		void SetObjList(std::map<int, std::shared_ptr<VFX_EffectObj>> list) { _objList = list; }
		std::map<int, std::shared_ptr<VFX_EffectObj>> GetObjList() { return _objList; }

		void SetEffectType(VFX_EFFECT_TYPE type) { _type = type; }
		VFX_EFFECT_TYPE GetEffectType() { return _type; }

		void SetActive(bool active) { _active = active; }
		bool GetActive() { return _active; }

		void SetScale(D3DXVECTOR3 scale) { _scale = scale; }
		D3DXVECTOR3 GetScale() { return _scale; }

		void SetRotate(D3DXVECTOR3 rotate) { _rotate = rotate; }
		D3DXVECTOR3 GetRotate() { return _rotate; }

		void SetPos(D3DXVECTOR3 pos) { _pos = pos; }
		D3DXVECTOR3 GetPos() { return _pos; }

		void setParticleScale(D3DXVECTOR3 scale);
		void setParticlePos(D3DXVECTOR3 scale);
		void setParticleLifeTimeLimit(float time);
		void setParticleSpeed(float speed);

		void SetParentMatWorld(D3DXMATRIX mat) { _parentMatWorld = mat; }
		D3DXMATRIX GetParentMatWorld() { return _parentMatWorld; }

	private: //member

		std::map<int, std::shared_ptr<VFX_EffectObj>> _objList;
		VFX_EFFECT_TYPE _type;

		D3DXMATRIX _parentMatWorld;
		D3DXMATRIX _matWorld;
		D3DXVECTOR3 _scale, _rotate, _pos;
		bool _active;
	};

}