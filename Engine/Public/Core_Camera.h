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
	CAMERA_DESC		Get_CAMERA_DESC() { return m_CameraDesc; }

protected:
	virtual void  Key_Input(_float fTimeDelta) {};
	virtual void  Camera_Event(_float fTimeDelta) {};
	void		  Set_Pos(const _float3 _vPos) { m_CameraDesc.vEye = _vPos; }

	virtual HRESULT Add_Components() PURE;

protected:
	CGameObject* m_pTarget = { nullptr };

	CTransform* m_pTransformCom = { nullptr };

	wstring		m_strKey;

	CAMERA_DESC m_CameraDesc;


	_float		m_MoveSensitivity = 1.f;

	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 _vOffset;

public:
	virtual void Free() override;

private:
	HRESULT Initialize();
};

END