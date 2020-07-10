#include "MyEffectParser.h"
#include "KG_DxHeper.h"
#include "VFX_ObjMgr.h"
#include "VFX_Effects.h"
#include "VFX_ScreenEffect.h"


KYS::MyEffectParser::MyEffectParser()
{
}

KYS::MyEffectParser::~MyEffectParser()
{
}

void KYS::MyEffectParser::LoadEffectDataFromFile(const wchar_t * fileName, ID3D11Device * device, ID3D11DeviceContext* context)
{

	_device = device;
	_context = context;

	std::stringstream dataList;
	LoadFile(fileName, &dataList);

	while (dataList)
	{
		std::stringstream data;
		char	name[256] = { 0 };
		wchar_t loadFIleName[256] = { 0 };

		dataList >> name;

		int len = MultiByteToWideChar(CP_ACP, 0, name, strlen(name), NULL, NULL);
		if (len <= 0) break;
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name), loadFIleName, len);


		LoadFile(loadFIleName, &data);
		CreateEffectObj(&data);
		
	}
	


}

void KYS::MyEffectParser::CreateEffectObj(std::stringstream * destData)
{
	KYS::VFX_EFFECT_INFO info;
	std::shared_ptr<VFX_EffectObj> vfxObj;
	KYS::VFX_EffectObj* obj;
	std::shared_ptr < KYS::VFX_Effects> effects;

	
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 initPos;
	D3DXVECTOR3 scale;
	D3DXMATRIX mat;
	D3DXVECTOR3 rotate;
	std::vector<MyParticle> particleList;

	int size = 0;
	int objcount = 0;
	char token;

	*destData >> objcount;
	
	effects = std::make_shared<KYS::VFX_Effects>();
	for (int count = 0; count < objcount; count++)
	{
		
	

		*destData >> info._blendType >> token;
		*destData >> info._billboard >> token;
		*destData >> info._effectType >> token;
		*destData >> info._active >> token;
		*destData >> info._activeLifeTime >> token;
		*destData >> info._lifeTimeLimit >> token;
		*destData >> info._activeInterval >> token;
		*destData >> info._activeFadeInOut >> token;
		*destData >> info._activeScaleRepeat >> token;
		*destData >> info._fadeInOutWeight >> token;
		*destData >> info._scaleRepeatWeight >> token;
		*destData >> info._interval;

		
		*destData >> info._position.x >> token;
		*destData >> info._position.y >> token;
		*destData >> info._position.z >> token;

		*destData >> info._rotation.x >> token;
		*destData >> info._rotation.y >> token;
		*destData >> info._rotation.z >> token;

		*destData >> info._scale.x >> token;
		*destData >> info._scale.y >> token;
		*destData >> info._scale.z >> token;
		if (info._effectType > 0)
		{
			vfxObj = std::make_shared<VFX_ScreenEffect>();
			obj = vfxObj.get();
		}
		else
		{
			vfxObj = std::make_shared<VFX_EffectObj>();
			obj = vfxObj.get();
		}
		
		Sprite_Info spriteInfo;
		SpriteTexture sprite;

		*destData >> spriteInfo._animType >> token;
		*destData >> spriteInfo._animTime >> token;
		*destData >> spriteInfo._renderTime >> token;
		*destData >> spriteInfo._widthCount >> token;
		*destData >> spriteInfo._heightCount >> token;
		*destData >> spriteInfo._widthSize >> token;
		*destData >> spriteInfo._heightSize >> token;
		*destData >> spriteInfo._textureCount >> token;

		*destData >> size;
		spriteInfo._textureList.resize(size);
		char name[256] = { 0 };
		wchar_t convertTextureName[256] = { 0 };
		for (auto& iter : spriteInfo._textureList)
		{
			ZeroMemory(name, sizeof(name));
			ZeroMemory(convertTextureName, sizeof(convertTextureName));

			*destData >> name;

			int len = MultiByteToWideChar(CP_ACP, 0, name, strlen(name), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, name, strlen(name), convertTextureName, len);

			iter = convertTextureName;

			int index = I_Texture.Add(_device, convertTextureName);
			spriteInfo._textureIndexList.push_back(index);
		}

		//shader
		wchar_t convertShaderName[256] = { 0 };
		ZeroMemory(name, sizeof(name));
		ZeroMemory(convertShaderName, sizeof(convertShaderName));
		*destData >> name;

		int len = MultiByteToWideChar(CP_ACP, 0, name, strlen(name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name), convertShaderName, len);

		//texture2
		wchar_t convertTextureName2[256] = { 0 };
		ZeroMemory(name, sizeof(name));
		ZeroMemory(convertTextureName2, sizeof(convertTextureName2));
		*destData >> name;
		if (strcmp(name, ","))
		{
			len = MultiByteToWideChar(CP_ACP, 0, name, strlen(name), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, name, strlen(name), convertTextureName2, len);
			spriteInfo._textureFile2 = convertTextureName2;
		}
		else
			spriteInfo._textureFile2 = L"";
		
		//normalTexture
		wchar_t convertNormalName[256] = { 0 };
		ZeroMemory(name, sizeof(name));
		ZeroMemory(convertNormalName, sizeof(convertNormalName));
		*destData >> name;
		if (strcmp(name, ","))
		{
			len = MultiByteToWideChar(CP_ACP, 0, name, strlen(name), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, name, strlen(name), convertNormalName, len);
			spriteInfo._normalTextureFile = convertNormalName;
		}
		else
			spriteInfo._normalTextureFile = L"";

		 

		/*if (!spriteInfo._animType)
		{
			*destData >> size;
			spriteInfo._uvList.resize(size);
			for (auto& iter : spriteInfo._uvList)
			{
				MyUV uv;
				uv = iter;
				*destData >> uv._uv[0].x >> token;
				*destData >> uv._uv[0].y >> token;

				*destData >> uv._uv[1].x >> token;
				*destData >> uv._uv[1].y >> token;

				*destData >> uv._uv[2].x >> token;
				*destData >> uv._uv[2].y >> token;

				*destData >> uv._uv[3].x >> token;
				*destData >> uv._uv[3].y >> token;
				iter = uv;
			}

		}*/
		sprite.setSpriteInfo(spriteInfo);
		obj->setSprite(sprite);

		*destData >> size;
		particleList.resize(size);

		info._particleCount = size;
		obj->setInfo(info);
		obj->setInitInfo(info);

		for (auto& iter : particleList)
		{
			ParticleInfo particleInfo;

			*destData >> particleInfo._lifeTime >> token;
			*destData >> particleInfo._lifeTimeLimit >> token;
			*destData >> particleInfo._gravity >> token;
			*destData >> particleInfo._moveSpeed >> token;
			*destData >> particleInfo._radius >> token;
			*destData >> particleInfo._radiusSpeed >> token;
			*destData >> particleInfo._activeLifeTime >> token;
			*destData >> particleInfo._activeGravity >> token;
			*destData >> particleInfo._active >> token;

			*destData >> particleInfo._initPos.x >> token;
			*destData >> particleInfo._initPos.y >> token;
			*destData >> particleInfo._initPos.z >> token;
			*destData >> particleInfo._pos.x >> token;
			*destData >> particleInfo._pos.y >> token;
			*destData >> particleInfo._pos.z >> token;
			*destData >> particleInfo._scale.x >> token;
			*destData >> particleInfo._scale.y >> token;
			*destData >> particleInfo._scale.z >> token;
			if (info._effectType > 0)
			{
				particleInfo._scale.x = Winrt.right / 2;
				particleInfo._scale.y = Winrt.bottom / 2;
			}
			*destData >> particleInfo._rotate.x >> token;
			*destData >> particleInfo._rotate.y >> token;
			*destData >> particleInfo._rotate.z >> token;

			*destData >> particleInfo._direction.x >> token;
			*destData >> particleInfo._direction.y >> token;
			*destData >> particleInfo._direction.z >> token;

			*destData >> particleInfo._initDirection.x >> token;
			*destData >> particleInfo._initDirection.y >> token;
			*destData >> particleInfo._initDirection.z >> token;

			*destData >> particleInfo._matWorld._11 >> token;
			*destData >> particleInfo._matWorld._12 >> token;
			*destData >> particleInfo._matWorld._13 >> token;
			*destData >> particleInfo._matWorld._14 >> token;
			*destData >> particleInfo._matWorld._21 >> token;
			*destData >> particleInfo._matWorld._22 >> token;
			*destData >> particleInfo._matWorld._23 >> token;
			*destData >> particleInfo._matWorld._24 >> token;
			*destData >> particleInfo._matWorld._31 >> token;
			*destData >> particleInfo._matWorld._32 >> token;
			*destData >> particleInfo._matWorld._33 >> token;
			*destData >> particleInfo._matWorld._34 >> token;
			*destData >> particleInfo._matWorld._41 >> token;
			*destData >> particleInfo._matWorld._42 >> token;
			*destData >> particleInfo._matWorld._43 >> token;
			*destData >> particleInfo._matWorld._44 >> token;
			iter.setInfo(particleInfo);
			iter.setInitInfo(particleInfo);
		}
		obj->setParticleList(particleList);

		obj->Create(_device, _context, convertShaderName, convertTextureName, "VS", "PS");
		obj->Init();

		effects->Add(vfxObj);
	}
	effects->Shutdown();
	VFX_MGR->add(effects, info._effectType);
	return ;
}

