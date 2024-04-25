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

	_float3 Get_Pos()
	{
		return *(_float3*)&m_WorldMatrix.m[STATE_POSITION][0];
	}

	_float3 Get_Look()
	{
		_float3 vLook = *(_float3*)&m_WorldMatrix.m[STATE_LOOK][0];
		D3DXVec3Normalize(&vLook, &vLook);
		return vLook;
	}
	_float3 Get_Right()
	{
		_float3 vRight = *(_float3*)&m_WorldMatrix.m[STATE_RIGHT][0];
		D3DXVec3Normalize(&vRight, &vRight);
		return vRight;
	}
	_float3 Get_Up()
	{
		_float3 vUp = *(_float3*)&m_WorldMatrix.m[STATE_UP][0];
		D3DXVec3Normalize(&vUp, &vUp);
		return vUp;
	}

	_float3 Get_GroundLook()
	{
		_float3 vLook = *(_float3*)&m_WorldMatrix.m[STATE_LOOK][0];
		vLook.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);
		return vLook;
	}

	_float3 Get_GroundRight()
	{
		_float3 vRight = *(_float3*)&m_WorldMatrix.m[STATE_RIGHT][0];
		vRight.y = 0.f;
		D3DXVec3Normalize(&vRight, &vRight);
		return vRight;
	}

	_float3 Get_Scale()
	{
		return _float3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}

	void Set_State(STATE eState, const _float3* pState) {
		memcpy(&m_WorldMatrix.m[eState][0], pState, sizeof(_float3));
		Update_Offset();
	}

	void Set_UnOffset_State(STATE eState, const _float3* pState) {
		memcpy(&m_WorldMatrix.m[eState][0], pState, sizeof(_float3));
	}

	void Debug_State_Out() {
		std::cout << "REAL Right :" << m_WorldMatrix._11 << " " << m_WorldMatrix._12 << " " << m_WorldMatrix._13 << std::endl;
		std::cout << "REAL Up :" << m_WorldMatrix._21 << " " << m_WorldMatrix._22 << " " << m_WorldMatrix._23 << std::endl;
		std::cout << "REAL Look :" << m_WorldMatrix._31 << " " << m_WorldMatrix._32 << " " << m_WorldMatrix._33 << std::endl;
		std::cout << "REAL Position :" << m_WorldMatrix._41 << " " << m_WorldMatrix._42 << " " << m_WorldMatrix._43 << std::endl;
		std::system("cls");
	}

	void Debug_State_Out2() {
		std::cout << "OFFSET Right :" << m_WorldMatrix_Offset._11 << " " << m_WorldMatrix_Offset._12 << " " << m_WorldMatrix_Offset._13 << std::endl;
		std::cout << "OFFSET Up :" << m_WorldMatrix_Offset._21 << " " << m_WorldMatrix_Offset._22 << " " << m_WorldMatrix_Offset._23 << std::endl;
		std::cout << "OFFSET Look :" << m_WorldMatrix_Offset._31 << " " << m_WorldMatrix_Offset._32 << " " << m_WorldMatrix_Offset._33 << std::endl;	
		std::cout << "OFFSET Position :" << m_WorldMatrix_Offset._41 << " " << m_WorldMatrix_Offset._42 << " " << m_WorldMatrix_Offset._43 << std::endl;
	}

	void Set_Scale(const _float3& vScale);
	void Multiply_Scale(const _float3& vScale);

	void Set_ScaleX(const _float fX);
	void Set_ScaleY(const _float fY);
	void Set_ScaleZ(const _float fZ);

	void Set_PosX(const _float& fX) { m_WorldMatrix.m[STATE_POSITION][0] = fX; }
	void Set_PosY(const _float& fY) { m_WorldMatrix.m[STATE_POSITION][1] = fY; }
	void Set_PosZ(const _float& fZ) { m_WorldMatrix.m[STATE_POSITION][2] = fZ; }

	void Add_Pos(const _float3& vAdd);
	
	void Set_Billboard_Matrix(_float4x4 _BillboardMatrix);	// y축 빌보드 행렬을 만들기 위한 함수
	
	void Set_Target(const _float3& vCurPoint, const _float3& vWorldPoint);	// 특정 위치를 목표점으로 잡는 함수

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

	const _float4x4& Get_WorldMatrix() const
	{
		return m_WorldMatrix;
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

	void Go_Floor_Straight(_float fTimeDelta);
	void Go_Floor_Backward(_float fTimeDelta);
	void Go_Floor_Right(_float fTimeDelta);
	void Go_Floor_Left(_float fTimeDelta);

	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	void Go_Jump(_float fTimeDelta, _float fJumpPower);

	void Head_Roll(_float fTimeDelta, _float Degree);

	void Set_View_RollBack();
	void Set_HeadUp_Initialize();


	void Turn(const _float3& vAxis, _float fTimeDelta);
	void UnOffset_Turn(const _float3& vAxis, _float fTimeDelta);

	void Rotation(const _float3& vAxis, _float fRadian);
	void Rotation_XYZ(const _float3& vXYZ); // XYZ Degree 값 받아서 돌아줌.

	void Set_RotationSpeed(_float fRadian) { m_fRotationPerSec = fRadian; }

	void LookAt(const _float3& vWorldPoint);
	void LookAt_ForLandObject(const _float3& vWorldPoint);
	
	void Set_Position(const _float3& vPos) {
		m_WorldMatrix._41 = vPos.x; m_WorldMatrix._42 = vPos.y; m_WorldMatrix._43 = vPos.z; 
		Update_Offset(); 
	}

	void Set_Pos(const _float3& vPos)
	{
		Set_State(STATE_POSITION, &vPos);
	}
private:
	void Update_Offset();

//Camera Action
public:
	void Camera_Shake(_float fTimeDelta, _float fShakePower);
	void Camera_Shake_Init();
	void Camera_Gun_Shake(_float fTimeDelta, _float fShakePower);
	void Camera_Shake_End();
public:
	void AddSpeedPerSec(_float Speed) { m_fSpeedPerSec += Speed; }
	void Set_Speed(_float Speed) { m_fSpeedPerSec = Speed; }
	_float Get_Speed() { return m_fSpeedPerSec; }
	void MonsterDieUi_Go_Up(_float fTimeDelta, _float Speed);

private:
	_float4x4			m_WorldMatrix;
	_float4x4			m_WorldMatrix_Offset;

private:
	_float				m_fSpeedPerSec = { 0.0f };
	_float				m_fRotationPerSec = { 0.0f };

	_float              m_fShakePowerReverse = 0.f;
	_int			    m_iShakeDirection = 0;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END