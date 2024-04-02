#pragma once

#include "Component.h"

/* 1. 월드 상태에서의 모든 상태변환을 제어한다. */
/* 2. 월드 변환 행렬을 보관한다. */
/* 3. 월드 변환이 필요한 모든 객체들이 보유하는 컴포너늩다. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scale()
	{
		return _float3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}

	void Set_State(STATE eState, const _float3* pState) {
		memcpy(&m_WorldMatrix.m[eState][0], pState, sizeof(_float3));
	}

	void Set_Scale(const _float3& vScale);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_WorldMatrix() {
		return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	}

	_float4x4 Get_WorldMatrix_Inverse()
	{
		return *D3DXMatrixInverse(&_float4x4{}, nullptr, &m_WorldMatrix);
	}

	void Set_WorldMatrix(const _float4x4& worldMatrix)
	{
		m_WorldMatrix = worldMatrix;
	}

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);


	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Rotation(const _float3& vAxis, _float fRadian);
	void Rotation_XYZ(const _float3& vXYZ); // XYZ Degree 값 받아서 돌아줌.

	void LookAt(const _float3& vWorldPoint);
	void LookAt_ForLandObject(const _float3& vWorldPoint);

public:
	void AddSpeedPerSec(_float Speed) { m_fSpeedPerSec += Speed; }
	void Set_Speed(_float Speed) { m_fSpeedPerSec = Speed; }
	void MonsterDieUi_Go_Up(_float fTimeDelta, _float Speed);

private:
	_float4x4			m_WorldMatrix;

private:
	_float				m_fSpeedPerSec = { 0.0f };
	_float				m_fRotationPerSec = { 0.0f };
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END