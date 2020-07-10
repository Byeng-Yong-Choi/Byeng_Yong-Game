#include "VFX_Effects.h"
#include "KG_Camera.h"

KYS::VFX_Effects::VFX_Effects()
{
	_type = VFX_EFFECT_TYPE::VFX_EFFECT_NONE;
	_active = true;
	D3DXMatrixIdentity(&_matWorld);
	D3DXMatrixIdentity(&_parentMatWorld);
	_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	_rotate= D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

KYS::VFX_Effects::VFX_Effects(VFX_Effects * obj)
{

}

KYS::VFX_Effects& KYS::VFX_Effects::operator=(KYS::VFX_Effects& _Right)
{	
	_type = _Right.GetEffectType();
	_parentMatWorld = _Right.GetParentMatWorld();
	_matWorld = _Right._matWorld;
	_scale = _Right.GetScale();
	_rotate = _Right.GetRotate();
	_pos = _Right.GetPos();
	_active = _Right.GetActive();

	std::shared_ptr<VFX_EffectObj> obj = nullptr;
	for (auto& iter : _Right.GetObjList())
	{
		obj = iter.second;
		std::shared_ptr<VFX_EffectObj> newObj = std::make_shared< VFX_EffectObj>();
		*newObj = *obj;
		newObj->Create(obj->m_obj.m_pd3dDevice, obj->m_obj.m_pContext, obj->GetShaderFileName().c_str(), obj->GetTextureFileName().c_str(),"VS","PS");
		newObj->Init();
		_objList.insert(std::make_pair(iter.first, newObj));
	}

	return *this;
}

KYS::VFX_Effects::~VFX_Effects()
{
}

bool KYS::VFX_Effects::Init()
{
	return false;
}

bool KYS::VFX_Effects::Frame()
{
	if (!_active) return false;

	UpdateMatrix();
	ApplyToChildren(_matWorld);

	for (auto obj : _objList)
	{
		if (!obj.second->getInfo()._active)
			continue;
		if (_type == VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_FIRE)
		{
			obj.second->FrameInterval();
			continue;
		}
			
		obj.second->Frame();
	}

	UpdateState();
	return true;
}

int KYS::VFX_Effects::Add(std::shared_ptr<VFX_EffectObj> obj)
{
	if (obj == nullptr)
		return -1;

	int index = -1;
	int size = 0;

	index = GetEffectObj(obj);
	size = _objList.size();

	if (index >= 0)
		return index;
	else
		_objList.insert(std::make_pair(size,obj));
	

	return size;
}

std::shared_ptr<KYS::VFX_EffectObj> KYS::VFX_Effects::GetEffectObj(int index)
{

	if (index < 0)
		return nullptr;

	std::shared_ptr<VFX_EffectObj> reObj;

	reObj = _objList.find(index)->second;

	return reObj;
}

int KYS::VFX_Effects::GetEffectObj(std::shared_ptr<VFX_EffectObj> obj)
{
	for (DWORD index = 0; index < _objList.size(); index++)
	{
		if (obj == _objList[index])
			return index;
	}
	return -1;
}

void KYS::VFX_Effects::Execute(D3DXMATRIX* parentMat, D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{
	ResetState();
	if (parentMat != nullptr)
		SetParentMatWorld(*parentMat);
	SetPos(pos);
	SetRotate(rotate);
	UpdateMatrix();
	ApplyToChildren(_matWorld);

	std::shared_ptr<KYS::VFX_EffectObj> obj;
	for (int index = 0; index < _objList.size(); index++)
	{
		obj = _objList[index];
		obj->Execute();
		if (_type == VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_FIRE)
		{
			if (index >= 2)
			{
				obj->ParticleReset(true);
			}
			else
			{
				obj->ParticleReset(false);
			}
		}
			
	}
	/*for (auto& obj : _objList)
	{
		obj.second->Execute();
		if (_type == VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_FIRE)

			obj.second->ParticleReset(false);
	}*/
}

void KYS::VFX_Effects::Shutdown()
{
	_active = false;
	for (auto& obj : _objList)
	{
		obj.second->shutdown();
	}
}

void KYS::VFX_Effects::UpdateState()
{
	bool active = true;
	for (auto obj : _objList)
	{
		active = obj.second->getInfo()._active;
		if (active) break;
	}

	if (!active) _active = false;
}

void KYS::VFX_Effects::UpdateMatrix()
{
	
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
	//D3DXMatrixIdentity(&matRotate);
	//D3DXMatrixRotationX(&matRotate, rotX);
	//D3DXMatrixRotationY(&matRotate, rotY);
	//D3DXMatrixRotationZ(&matRotate, rotZ);
	
	//D3DXQuaternionRotationYawPitchRoll(&quaterRotate, rotY, rotX, rotZ);
	D3DXMatrixRotationYawPitchRoll(&matRotate, rotY, rotX, rotZ);
	//D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, NULL);
	//D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, _pos);

	D3DXMatrixScaling(&matScale, _scale.x, _scale.y, _scale.z);

	matTrans._41 = _pos.x;
	matTrans._42 = _pos.y;
	matTrans._43 = _pos.z;

	//부모 행렬분해
	D3DXMatrixDecompose(&parentScale, &parentQuaterRotate, &parentTrans, &_parentMatWorld);
	D3DXMatrixRotationQuaternion(&parentMatRotate, &parentQuaterRotate);
	parentMatRotate._41 = parentTrans.x ;
	parentMatRotate._42 = parentTrans.y ;
	parentMatRotate._43 = parentTrans.z;
	/*parentMatTrans._41 = parentTrans.x;
	parentMatTrans._42 = parentTrans.y;
	parentMatTrans._43 = parentTrans.z;*/

	//m_matWorld = matScale * matRotate * matTrans * _parentMatWorld;
	_matWorld = matScale * matRotate * matTrans * parentMatRotate;
	
}

void KYS::VFX_Effects::ApplyToChildren(D3DXMATRIX mat)
{

	for (auto& obj : _objList)
	{
		obj.second->SetParentMatWorld(mat);
		obj.second->UpdateMatWorld();
	}
}

void KYS::VFX_Effects::ResetState()
{
	_active = true;
	if ( _type != VFX_EFFECT_TYPE::VFX_EFFECT_SCREEN_BLOOD_IMPACT)
		D3DXMatrixIdentity(&_parentMatWorld);

}

void KYS::VFX_Effects::setParticleScale(D3DXVECTOR3 scale)
{
	for (auto& obj : _objList)
	{
		obj.second->setParticleScale(scale);
	}
}

void KYS::VFX_Effects::setParticlePos(D3DXVECTOR3 pos)
{
	for (auto& obj : _objList)
	{
		obj.second->setParticlePos(pos);
	}
}

void KYS::VFX_Effects::setParticleLifeTimeLimit(float time)
{
	for (auto& obj : _objList)
	{
		obj.second->setParticleLifeTimeLimit(time);
	}
}

void KYS::VFX_Effects::setParticleSpeed(float speed)
{
	std::shared_ptr<VFX_EffectObj> obj = nullptr;
	for (int index = 0; index < _objList.size(); index++)
	{
		if (index < 2)
		{
			obj = _objList[index];
			obj->setParticleSpeed(speed);
		}
	}
	/*for (auto& obj : _objList)
	{
		obj.second->setParticleSpeed(speed);
	}*/
}

//void KYS::VFX_Effects::SetScale(D3DXVECTOR3 scale)
//{
//	/*ParticleInfo info;
//	for (auto& obj : _objList)
//	{
//		obj.second->setParticleScale(scale);
//	}*/
//}
//
//D3DXVECTOR3 KYS::VFX_Effects::GetScale()
//{
//	return D3DXVECTOR3();
//}



bool KYS::VFX_Effects::Render(KG_Camera* camera, ID3D11Device* _device,ID3D11DeviceContext* _context)
{
	if (!_active) return false;

	D3DXMATRIX matViewInverse;
	D3DXMATRIX matBillBoard;
	D3DXMATRIX matWorld;
	D3DXMATRIX objMat;
	D3DXVECTOR3 objPos;
	D3DXVECTOR3 trans;
	ParticleInfo info;
	std::shared_ptr<VFX_EffectObj> obj;

	ID3D11RasterizerState* oldRaster = nullptr;
	_context->RSGetState(&oldRaster);
	//컬링 none
	CDX::ApplyRS(_context, CDX::KG_DxState::g_pRSSold);
	

	for (auto effectObj : _objList)
	{
		obj = effectObj.second;

		if (!obj->getInfo()._active)
			continue;
		//blend
		int blendType = obj->getInfo()._blendType;
		CDX::ApplyDSS(_context, CDX::KG_DxState::g_pDSSMaskZero);
		
		
		for (auto& iter : obj->getParticleList())
		{
			if (!iter.getInfo()._active)
				continue;
		
			if (obj->getInfo()._billboard)
			{
				//	//billboard

				info = iter.getInfo();
				matWorld = iter.getInfo()._matWorld;
				objMat = obj->GetMatWorld();
				objPos.x = objMat._41;
				objPos.y = objMat._42;
				objPos.z = objMat._43;

				matViewInverse = camera->m_View;
				D3DXMatrixInverse(&matViewInverse, NULL, &matViewInverse);

				matViewInverse._41 = 0;
				matViewInverse._42 = 0;
				matViewInverse._43 = 0;
				matViewInverse._44 = 1;

				matWorld._41 = 0;
				matWorld._42 = 0;
				matWorld._43 = 0;
				matWorld._44 = 1;

				objMat._41 = 0;
				objMat._42 = 0;
				objMat._43 = 0;
				objMat._44 = 1;


				//matBillBoard = matWorld * objMat * matViewInverse;
				//matBillBoard = matWorld * matViewInverse;

			

				D3DXMATRIX matScale, matRotate, matTrans;
				D3DXQUATERNION quaterRotate;
				D3DXMATRIX parentMatRotate, parentMatScale, parentMatWorld;
				D3DXQUATERNION parentQuaterRotate;
				D3DXVECTOR3 parentScale, parentTrans;

				D3DXMatrixIdentity(&matScale);
				D3DXMatrixIdentity(&matRotate);
				D3DXMatrixIdentity(&matTrans);
				D3DXMatrixIdentity(&parentMatRotate);
				D3DXMatrixIdentity(&parentMatScale);
				D3DXMatrixIdentity(&parentMatWorld);

				float rotX = D3DXToRadian(info._rotate.x);
				float rotY = D3DXToRadian(info._rotate.y);
				float rotZ = D3DXToRadian(info._rotate.z);

				D3DXQuaternionRotationYawPitchRoll(&quaterRotate, rotY, rotX, rotZ);
				D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, &info._pos);

				D3DXMatrixScaling(&matScale, info._scale.x, info._scale.y, info._scale.z);

				matTrans._41 = info._pos.x;
				matTrans._42 = info._pos.y;
				matTrans._43 = info._pos.z;



				D3DXMatrixDecompose(&parentScale, &parentQuaterRotate, &parentTrans, &objMat);
				D3DXMatrixRotationQuaternion(&parentMatRotate, &parentQuaterRotate);
				D3DXMatrixScaling(&parentMatScale, parentScale.x, parentScale.y, parentScale.z);
				parentMatWorld = parentMatScale;


				matWorld = matScale * matRotate * matTrans * parentMatWorld;
				matBillBoard = matWorld * matViewInverse;

				matBillBoard._41 += objPos.x + info._pos.x;
				matBillBoard._42 += objPos.y + info._pos.y;
				matBillBoard._43 += objPos.z + info._pos.z;

				obj->SetMatrix(&matBillBoard,
					camera);
			}
			else
			{

				info = iter.getInfo();
				matWorld = info._matWorld;

				objMat = obj->GetMatWorld();

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

				float rotX = D3DXToRadian(info._rotate.x);
				float rotY = D3DXToRadian(info._rotate.y);
				float rotZ = D3DXToRadian(info._rotate.z);

				D3DXQuaternionRotationYawPitchRoll(&quaterRotate, rotY, rotX, rotZ);
				D3DXMatrixAffineTransformation(&matRotate, 1.0f, NULL, &quaterRotate, NULL);

				D3DXMatrixScaling(&matScale, info._scale.x, info._scale.y, info._scale.z);

				matTrans._41 = info._pos.x;
				matTrans._42 = info._pos.y;
				matTrans._43 = info._pos.z;

				/*D3DXMatrixDecompose(&parentScale, &parentQuaterRotate, &parentTrans, &objMat);
				D3DXMatrixRotationQuaternion(&parentMatRotate, &parentQuaterRotate);*/
				matWorld = matScale * matRotate *  matTrans * objMat;

				//matWorld = matWorld * objMat;

				obj->SetMatrix(&matWorld,
					camera);
			}
			CDX::ApplyBS(_context, CDX::KG_DxState::g_pBlendState[blendType]);
			obj->Render();

		}
	}
	CDX::ApplyDSS(_context, CDX::KG_DxState::g_pDSS);
	CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlendDisable);
	//이전 레스터라이즈 상태로 되돌림 
	CDX::ApplyRS(_context, oldRaster);

	return true;
}

bool KYS::VFX_Effects::Release()
{
	for (auto obj : _objList)
	{
		obj.second->Release();
	}
	return false;
}
