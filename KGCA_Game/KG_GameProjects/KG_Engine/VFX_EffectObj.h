#pragma once
#include "KG_ShapeObject.h"
#include "SpriteTexture.h"
#include "MyParticle.h"



namespace KYS
{
	struct VFX_EFFECT_INFO
	{
		D3DXVECTOR3 _direction;
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _scale;
		D3DXVECTOR3 _rotation;

		int			_blendType;
		int			_effectType;
		int			_particleCount;
		float		_lifeTime;
		float		_lifeTimeLimit;
		float		_interval;
		float _fadeInOutWeight;
		float _scaleRepeatWeight;

		bool		_active;
		bool		_activeLifeTime;
		bool		_activeInterval;
		bool		_activeFadeInOut;
		bool		_activeScaleRepeat;
		bool		_billboard;
		
		VFX_EFFECT_INFO()
		{
			_effectType = _blendType = _particleCount = 0;
			_scaleRepeatWeight = _fadeInOutWeight = _lifeTimeLimit = _lifeTime = _interval =  0.0f;
			_direction = _position = _scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			_activeScaleRepeat = _activeFadeInOut = _activeLifeTime = _activeInterval = _billboard = _active = false;
		}

	};
	struct ConstantBuffer_Effect
	{
		//hlsl에서 bool은 int와 같은 4바이트
		int	_activeFadeInOut;
		int	_activeScaleRepeat;

		float _fadeInOutWeight;
		float _scaleRepeatWeight;

		ConstantBuffer_Effect()
		{
			_activeFadeInOut = _activeScaleRepeat = false;
			_fadeInOutWeight = _scaleRepeatWeight = 0.0f;
		}
	};
}
class KG_Camera;
namespace KYS
{
	class VFX_EffectObj : public KG_ShapeObject
	{
	public:
		VFX_EffectObj();
		virtual ~VFX_EffectObj();
		KYS::VFX_EffectObj& operator= (VFX_EffectObj& _Right);
	public:
		virtual bool Init()override;
		virtual bool Frame()override;
		virtual bool FrameInterval();
		virtual bool Render()override;
		virtual bool Release()override;

		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
		virtual void SetMatrix(D3DXMATRIX* world, KG_Camera* camera) ;

		virtual bool Create(
			ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, 
			const TCHAR* ShaderFileName = L"../../data/shader/DefaultShader.txt", 
			const TCHAR* TexFileName = nullptr, 
			const CHAR* VSName = "VSmat", const CHAR* PSName = "PS") override;
		
	public:
		//컨테이너에서 재사용가능 한 파티클 시작 인덱스 반환.
		void findReusableParticle();
		//재사용가능한 파티클이 없을 시 다시 만든다.
		void ReCreateParticle();
		//재사용가능한 파티클을 초기화
		void ResetReusableParticle();
		void ParticleReset(bool _activeState);
		void Execute();
		void shutdown();
		void resetState();
		void createConstantBuffer();
		void updateConstantBuffer();
		void createSRV();
		void UpdateMatWorld();
		HRESULT LoadTexture(const TCHAR* fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV);
	public:

		void setInfo(VFX_EFFECT_INFO info) { _info = info; }
		VFX_EFFECT_INFO getInfo() { return _info; }

		void setInitInfo(VFX_EFFECT_INFO info) { _initInfo = info; }
		VFX_EFFECT_INFO getInitInfo() { return _initInfo; }

		void setSprite(SpriteTexture sprite) { _sprite = sprite; }
		SpriteTexture getSprite() { return _sprite; }

		void setParticleList(std::vector<MyParticle> list) { _particleList = list; }
		std::vector<MyParticle> getParticleList() { return _particleList; }

		void setPos(D3DXVECTOR3 pos) { _pos = pos; }
		D3DXVECTOR3 getPos() { return _pos; }

		void setRotate(D3DXVECTOR3 rotate) { _rotate = rotate; }
		D3DXVECTOR3 getRotate() { return _rotate; }

		void setScale(D3DXVECTOR3 scale) { _scale = scale; }
		D3DXVECTOR3 getScale() { return _scale; }

		void setParticleScale(D3DXVECTOR3 scale);
		D3DXVECTOR3 getParticleScale();
		void setParticlePos(D3DXVECTOR3 pos);
		void setParticleLifeTimeLimit(float time);
		void setParticleSpeed(float speed);

		void SetMatWorld(D3DXMATRIX mat) { m_matWorld = mat; }
		D3DXMATRIX GetMatWorld() { return m_matWorld; }

		void SetParentMatWorld(D3DXMATRIX mat) { _parentMatWorld = mat; }
		D3DXMATRIX GetParentMatWorld() { return _parentMatWorld; }

		float GetAccScale() { return _accScale; }
		float GetAccFade() { return _accFade; }

		std::wstring& GetShaderFileName() { return _shaderFileName; }
		std::wstring& GetTextureFileName() { return _textureFileName; }
	protected:
		VFX_EFFECT_INFO _info;
		VFX_EFFECT_INFO _initInfo;
		SpriteTexture _sprite;
		D3DXVECTOR3 _rotate;
		D3DXVECTOR3 _scale;
		D3DXVECTOR3 _pos;
		D3DXMATRIX _parentMatWorld;
		float _time;
		float _accScale;
		float _accFade;
		std::wstring _shaderFileName;
		std::wstring _textureFileName;

		ID3D11ShaderResourceView* _animationSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _srv2;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _normalSrv;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _effectConstantBuffer;
		ConstantBuffer_Effect _effectConstantData;

	protected:
		std::vector<MyParticle> _particleList;
		std::vector<int> _reusableParticleList;
	};
}


