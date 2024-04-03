#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class CTransform;
class ENGINE_DLL CCoreCamera : public CGameObject
{

public:
	typedef struct CamDesc : public CTransform::TRANSFORM_DESC
	{
		_float3			vEye;
		_float3			vAt;

		_float			fFovy;
		_float			fNear;
		_float			fFar;
		_float			fMouseSensor;

	}CAMERA_DESC;

public:
	CCoreCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCoreCamera(const CCoreCamera& rhs);
	wstring Get_Key() const { return m_strKey; }

	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
protected:

	//LPDIRECT3DDEVICE9	 m_pGraphic_Device = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

	void	Set_Key(const wstring& _strKey) { m_strKey = _strKey; }
	void	Set_Pos(const _float3 _vPos) { m_CameraDesc.vEye = _vPos; }

protected:
	CGameObject* m_pTarget = { nullptr };

	CTransform* m_pTransformCom = { nullptr };

	wstring m_strKey;

	CAMERA_DESC m_CameraDesc;

	_float _fFovY;
	_float _fAspect;
	_float _fFar;
	_float _fNear;

	_float _fYaw;
	_float _fPitch;
	_float _fRoll;
	_float _fOffset;
	_float _fSensitive;

	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 _vOffset;

public:
	virtual void Free() override;

private:
	HRESULT Initialize();
};

END