#include "VFX_EffectObj.h"
#include "KG_Camera.h"

KYS::VFX_EffectObj::VFX_EffectObj()
{
	_animationSRV = nullptr;
	 _accFade = _accScale = 0.0f;
	 D3DXMatrixIdentity(&_parentMatWorld);
	 _rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	 _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	 _pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	 _time = 0.0f;
}

KYS::VFX_EffectObj::~VFX_EffectObj()
{
}

KYS::VFX_EffectObj& KYS::VFX_EffectObj::operator=(KYS::VFX_EffectObj& _Right)
{
	_info = _Right.getInitInfo();
	_initInfo = _Right.getInitInfo();
	_sprite = _Right.getSprite();
	_rotate = _Right.getRotate();
	_scale = _Right.getScale();
	_pos = _Right.getPos();
	_parentMatWorld = _Right.GetParentMatWorld();
	_accScale = _Right.GetAccScale();
	_accFade = _Right.GetAccFade();


	_particleList.resize(_info._particleCount);
	for (int index = 0; index < _info._particleCount; index++)
	{
		_particleList[index] = _Right.getParticleList()[index];
	}
	
	return *this;
}

bool KYS::VFX_EffectObj::Init()
{
	_sprite.createTextureAnimInfo();
	_scale = _info._scale;
	_rotate = _info._rotation;
	_pos = _info._position;
	createSRV();
	createConstantBuffer();
	UpdateMatWorld();
	return false;
}

bool KYS::VFX_EffectObj::Frame()
{
	UpdateMatWorld();
	_time += g_SecondTime;

	_sprite.Frame();
	if (_sprite.getSpriteInfo()._animType)
	{
		int index = _sprite.getSpriteInfo()._textureIndexList[_sprite.getSpriteInfo()._curTextureIndex];
		CTexture* tempTexture = I_Texture.GetPtr(index);
		if (tempTexture != nullptr)
			_animationSRV = tempTexture->m_pTextureRV;
	}
	else
	{
		MyUV uvPos;
		uvPos = _sprite.getTextureUv(_sprite.getSpriteInfo()._curTextureIndex);

		for (int index = 0; index < 4; index++)
		{
			m_VerTex[index].t = uvPos._uv[index];
		}
		m_obj.m_pContext->UpdateSubresource(
			m_obj.m_pVertexBuffer.Get(),
			0, nullptr,
			&m_VerTex.at(0), 0, 0);
	}

	int activeCount = 0;
	for (auto& iter : _particleList)
	{
		if (!iter.getInfo()._active)
			continue;
		activeCount++;
		iter.frame();
	}
	if(activeCount <=0)
		_info._active = false;


	/*if (_info._activeLifeTime)
	{
		_info._lifeTime += g_SecondTime;
		if (_info._lifeTime >= _info._lifeTimeLimit)
		{
			_info._active = false;
			_info._activeLifeTime = false;
		}
	}*/

	if (_info._activeScaleRepeat)
	{
		_accScale += _info._scaleRepeatWeight * g_SecondTime;
		_effectConstantData._scaleRepeatWeight = (sinf(_accScale) + 1) / 2;
	}

	if (_info._activeFadeInOut)
	{
		_accFade += (_info._fadeInOutWeight *4) * g_SecondTime  ;
		_effectConstantData._fadeInOutWeight = (sinf(_accFade) + 1) / 2;
	}



	if (!_info._activeInterval) return true;

	

	if (_time > _info._interval)
	{
		_time = 0.0f;

		findReusableParticle();
		if (_reusableParticleList.size() < _info._particleCount)
		{
			ReCreateParticle();
		}
		else
			ResetReusableParticle();

		_reusableParticleList.clear();
		_reusableParticleList.reserve(_info._particleCount);
	}


	return false;
}

bool KYS::VFX_EffectObj::FrameInterval()
{
	UpdateMatWorld();
	_time += g_SecondTime;

	_sprite.Frame();
	if (_sprite.getSpriteInfo()._animType)
	{
		int index = _sprite.getSpriteInfo()._textureIndexList[_sprite.getSpriteInfo()._curTextureIndex];
		CTexture* tempTexture = I_Texture.GetPtr(index);
		if (tempTexture != nullptr)
			_animationSRV = tempTexture->m_pTextureRV;
	}
	else
	{
		MyUV uvPos;
		uvPos = _sprite.getTextureUv(_sprite.getSpriteInfo()._curTextureIndex);

		for (int index = 0; index < 4; index++)
		{
			m_VerTex[index].t = uvPos._uv[index];
		}
		m_obj.m_pContext->UpdateSubresource(
			m_obj.m_pVertexBuffer.Get(),
			0, nullptr,
			&m_VerTex.at(0), 0, 0);
	}

	int activeCount = 0;
	for (auto& iter : _particleList)
	{
		if (!iter.getInfo()._active)
			continue;
		activeCount++;
		iter.frame();
	}
	//if (activeCount <= 0)
	//	_info._active = false;


	/*if (_info._activeLifeTime)
	{
		_info._lifeTime += g_SecondTime;
		if (_info._lifeTime >= _info._lifeTimeLimit)
		{
			_info._active = false;
			_info._activeLifeTime = false;
		}
	}*/

	if (_info._activeScaleRepeat)
	{
		_accScale += _info._scaleRepeatWeight * g_SecondTime;
		_effectConstantData._scaleRepeatWeight = (sinf(_accScale) + 1) / 2;
	}

	if (_info._activeFadeInOut)
	{
		_accFade += (_info._fadeInOutWeight * 4) * g_SecondTime;
		_effectConstantData._fadeInOutWeight = (sinf(_accFade) + 1) / 2;
	}



	if (!_info._activeInterval) return true;



	if (_time > _info._interval)
	{
		_time = 0.0f;

		findReusableParticle();
		if (_reusableParticleList.size() < _info._particleCount)
		{
			ReCreateParticle();
		}
		else
			ResetReusableParticle();

		_reusableParticleList.clear();
		_reusableParticleList.reserve(_info._particleCount);
	}


	return false;
}

bool KYS::VFX_EffectObj::Render()
{
	m_obj.PrePender();

	m_obj.m_pContext->VSSetConstantBuffers(1, 1, _effectConstantBuffer.GetAddressOf());
	m_obj.m_pContext->PSSetConstantBuffers(1, 1, _effectConstantBuffer.GetAddressOf());

	if (_animationSRV != nullptr)
		m_obj.m_pContext->PSSetShaderResources(0, 1, &_animationSRV);

	m_obj.PostPender();
	return false;
}

bool KYS::VFX_EffectObj::Release()
{
	return false;
}

void KYS::VFX_EffectObj::SetMatrix(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	//KG_Model::SetMatrix(world, view, proj);

	D3DXMatrixTranspose(&m_ConData.World, world);
	D3DXMatrixTranspose(&m_ConData.View, view);
	D3DXMatrixTranspose(&m_ConData.Proj, proj);


	if (m_obj.iCBBufferType == CB_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE mss;
		if (m_obj.m_pConstantBuffer != nullptr)
		{
			if (SUCCEEDED(m_obj.m_pContext->Map(m_obj.m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mss)))
			{
				CONDATA* pData = (CONDATA*)mss.pData;
				memcpy(pData, &m_ConData, sizeof(CONDATA));
				m_obj.m_pContext->Unmap(m_obj.m_pConstantBuffer.Get(), 0);
			}
		}
	}
	else if (m_obj.iCBBufferType == CB_DEFAULT)
	{
		if (m_obj.m_pConstantBuffer != nullptr)
		{
			m_obj.m_pContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_ConData, 0, 0);
		}
	}

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;
	m_vSide.x = m_matWorld._11;
	m_vSide.y = m_matWorld._12;
	m_vSide.z = m_matWorld._13;
	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	updateConstantBuffer();
}

void KYS::VFX_EffectObj::SetMatrix(D3DXMATRIX * world, KG_Camera * camera)
{
	SetMatrix(world, &camera->m_View, &camera->m_Proj);
}

bool KYS::VFX_EffectObj::Create(ID3D11Device * pd3dDevice, ID3D11DeviceContext * Context, const TCHAR * ShaderFileName, const TCHAR * TexFileName, const CHAR * VSName, const CHAR * PSName)
{
	_shaderFileName = ShaderFileName;
	_textureFileName = TexFileName;
	
	KG_Model::Create(pd3dDevice, Context, ShaderFileName, TexFileName, VSName, PSName);

	return true;
}

void KYS::VFX_EffectObj::findReusableParticle()
{
	ParticleInfo tempInfo;
	int reusableParticleNumber = -1;

	for (int index = 0; index < _particleList.size(); index++)
	{
		tempInfo = _particleList[index].getInfo();
		if (!tempInfo._active)
		{
			if (_reusableParticleList.size() >= _info._particleCount) break;
			_reusableParticleList.push_back(index);
		}
	}

}

void KYS::VFX_EffectObj::ReCreateParticle()
{
	ParticleInfo tempInfo;
	

	for (int index = 0; index < _info._particleCount; index++)
	{
		MyParticle particleObj;
		tempInfo = _particleList[index].getInitInfo();
		tempInfo._scale = _particleList[index].getInfo()._scale;
		particleObj.setInfo(tempInfo);
		particleObj.setInitInfo(tempInfo);
		_particleList.push_back(particleObj);
	}
}

void KYS::VFX_EffectObj::ResetReusableParticle()
{
	int findedIndex = 0;
	for (int index = 0; index < _reusableParticleList.size(); index++)
	{
		findedIndex = _reusableParticleList[index];
		_particleList[findedIndex].resetState();
	}
}

void KYS::VFX_EffectObj::ParticleReset(bool _activeState)
{
	//m_vPos = pos;
	ParticleInfo info;
	
	for (auto& obj : _particleList)
	{
		obj.resetState();
		info = obj.getInfo();
		info._active = _activeState;
		obj.setInfo(info);
	}
}

void KYS::VFX_EffectObj::createConstantBuffer()
{
	_effectConstantBuffer = CDXH::CreateConstantBuffer(
		m_obj.m_pd3dDevice, nullptr, 1, sizeof(ConstantBuffer_Effect), true);

	_effectConstantData._activeFadeInOut = _info._activeFadeInOut;
	_effectConstantData._activeScaleRepeat = _info._activeScaleRepeat;
}

void KYS::VFX_EffectObj::updateConstantBuffer()
{
	if (_effectConstantBuffer != nullptr)
	{
		D3D11_MAPPED_SUBRESOURCE mss;
		if (SUCCEEDED(m_obj.m_pContext->Map(_effectConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mss)))
		{
			ConstantBuffer_Effect* pData = (ConstantBuffer_Effect*)mss.pData;
			memcpy(pData, &_effectConstantData, sizeof(ConstantBuffer_Effect));
			m_obj.m_pContext->Unmap(_effectConstantBuffer.Get(), 0);
		}

		/*m_obj.m_pContext->UpdateSubresource(_effectConstantBuffer.Get(),
			0, NULL, &_effectConstantData, 0, 0);*/
	}
}

void KYS::VFX_EffectObj::createSRV()
{
	if (!_sprite.getSpriteInfo()._textureFile2.empty())
	{
		LoadTexture(_sprite.getSpriteInfo()._textureFile2.c_str(), _srv2);
	}

	if (!_sprite.getSpriteInfo()._normalTextureFile.empty())
	{
		LoadTexture(_sprite.getSpriteInfo()._normalTextureFile.c_str(), _normalSrv);
	}

}

void KYS::VFX_EffectObj::UpdateMatWorld()
{
	//D3DXMATRIX matScale, matRotate;
	//D3DXMatrixIdentity(&matScale);
	//D3DXMatrixIdentity(&matRotate);

	//float rotX = D3DXToRadian(_rotate.x);
	//float rotY = D3DXToRadian(_rotate.y);
	//float rotZ = D3DXToRadian(_rotate.z);
	//D3DXMatrixIdentity(&matRotate);
	//D3DXMatrixRotationX(&matRotate, rotX);
	//D3DXMatrixRotationY(&matRotate, rotY);
	//D3DXMatrixRotationZ(&matRotate, rotZ);

	//D3DXMatrixScaling(&matScale,1.0f,1.0f,1.0f);
	//m_matWorld = matScale * matRotate;
	//m_matWorld._41 = m_vPos.x;
	//m_matWorld._42 = m_vPos.y;
	//m_matWorld._43 = m_vPos.z;

	//m_matWorld =  m_matWorld * _parentMatWorld ;

	D3DXMATRIX matScale, matRotate, matTrans;
	D3DXQUATERNION quaterRotate;
	D3DXMATRIX parentMatRotate, parentMatTrans;
	D3DXQUATERNION parentQuaterRotate;
	D3DXVECTOR3 parentScale, parentTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&parentMatRotate);
	D3DXMatrixIdentity(&parentMatTrans);

	float rotX = D3DXToRadian(_rotate.x);
	float rotY = D3DXToRadian(_rotate.y);
	float rotZ = D3DXToRadian(_rotate.z);

	D3DXQuaternionRotationYawPitchRoll(&quaterRotate, rotY, rotX, rotZ);
	//D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, &_pos);
	D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, NULL);

	D3DXMatrixScaling(&matScale, _scale.x, _scale.y, _scale.z);

	matTrans._41 = _pos.x;
	matTrans._42 = _pos.y;
	matTrans._43 = _pos.z;

	//부모 행렬분해
	D3DXMatrixDecompose(&parentScale, &parentQuaterRotate, &parentTrans, &_parentMatWorld);
	D3DXMatrixRotationQuaternion(&parentMatRotate, &parentQuaterRotate);
	parentMatRotate._41 = parentTrans.x;
	parentMatRotate._42 = parentTrans.y;
	parentMatRotate._43 = parentTrans.z;
	/*parentMatTrans._41 = parentTrans.x;
	parentMatTrans._42 = parentTrans.y;
	parentMatTrans._43 = parentTrans.z;*/

	//m_matWorld = matScale * matRotate * matTrans * _parentMatWorld;
	m_matWorld = matScale * matRotate * matTrans * parentMatRotate;
}

HRESULT KYS::VFX_EffectObj::LoadTexture(const TCHAR * fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV)
{
	HRESULT hr = S_OK;
	if (fileName == NULL) return S_OK;
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	_tsplitpath_s(fileName, Drive, Dir, FName, Ext);
	DirectX::TexMetadata imageMetadata;
	DirectX::ScratchImage* pScratchImage = new DirectX::ScratchImage();

	if (_tcsicmp(Ext, _T(".tga")) == 0)
	{
		hr = DirectX::LoadFromTGAFile(fileName, &imageMetadata, *pScratchImage);
		hr = DirectX::CreateShaderResourceView(m_obj.m_pd3dDevice, pScratchImage->GetImages(), pScratchImage->GetImageCount(), pScratchImage->GetMetadata(), SRV.GetAddressOf());
	}
	else
	{
		hr = D3DX11CreateShaderResourceViewFromFile(m_obj.m_pd3dDevice, fileName, NULL, NULL, SRV.GetAddressOf(), NULL);
	}

	if (pScratchImage != nullptr)
		delete pScratchImage;
	return hr;
}

void KYS::VFX_EffectObj::Execute()
{

	resetState();

	//m_vPos = pos;
	ParticleInfo info;
	for (auto& obj : _particleList)
	{
		obj.resetState();
		info = obj.getInfo();
		//info._active = false;
		obj.setInfo(info);
	}

	_sprite.resetState();


}

void KYS::VFX_EffectObj::shutdown()
{
	_info._active = false;

	for (auto& obj : _particleList)
	{
		obj.resetState();
	}
}

void KYS::VFX_EffectObj::resetState()
{
	_info = _initInfo;
}

void KYS::VFX_EffectObj::setParticleScale(D3DXVECTOR3 scale)
{
	ParticleInfo info;
	for (auto& obj : _particleList)
	{
		info = obj.getInfo();
		info._scale = scale;
		obj.setInfo(info);
		obj.matUpdate();
	}
}

D3DXVECTOR3 KYS::VFX_EffectObj::getParticleScale()
{
	return D3DXVECTOR3();
}

void KYS::VFX_EffectObj::setParticlePos(D3DXVECTOR3 pos)
{
	ParticleInfo info;
	D3DXVECTOR3 calPos;
	for (auto& obj : _particleList)
	{
		info = obj.getInitInfo();
		calPos = pos - info._pos;
		info._pos = info._pos + calPos;
		obj.setInitInfo(info);
	}
}

void KYS::VFX_EffectObj::setParticleLifeTimeLimit(float time)
{
	ParticleInfo info;
	for (auto& obj : _particleList)
	{
		info = obj.getInitInfo();
		info._lifeTimeLimit = time;
		obj.setInitInfo(info);
		obj.matUpdate();
	}
}

void KYS::VFX_EffectObj::setParticleSpeed(float speed)
{
	ParticleInfo info;
	//MyParticle obj;
	//for (int index = 0; index < _particleList.size(); index++)
	//{
	//	if (index < 2)
	//	{
	//		obj = _particleList[index];
	//		info = obj.getInitInfo();
	//		info._moveSpeed = speed;
	//		obj.setInitInfo(info);
	//		obj.matUpdate();

	//	}
	//}
	for (auto& obj : _particleList)
	{
		
		info = obj.getInitInfo();
		info._moveSpeed = speed;
		obj.setInitInfo(info);
		obj.matUpdate();
	}
}
