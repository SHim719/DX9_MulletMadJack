#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent{ rhs }
	, m_WorldMatrix{ rhs.m_WorldMatrix }
{
}

void CTransform::Multiply_Scale(const _float3& vScale)
{
	Set_State(STATE_RIGHT, &(Get_State(STATE_RIGHT) * vScale.x));
	Set_State(STATE_UP, &(Get_State(STATE_UP) * vScale.y));
	Set_State(STATE_LOOK, &(Get_State(STATE_LOOK) * vScale.z));
}

void CTransform::Set_ScaleX(const _float fX)
{
	_float3 vRight = *D3DXVec3Normalize(&vRight, &Get_State(STATE_RIGHT));
	if (D3DXVec3Length(&vRight) == 0.f)
		vRight.x = 1.f;
	vRight *= fX;

	Set_State(STATE_RIGHT, &vRight);
}

void CTransform::Set_ScaleY(const _float fY)
{
	_float3 vUp = *D3DXVec3Normalize(&vUp, &Get_State(STATE_UP));
	if (D3DXVec3Length(&vUp) == 0.f)
		vUp.y = 1.f;

	vUp *= fY;
	Set_State(STATE_UP, &vUp);
}

void CTransform::Set_ScaleZ(const _float fZ)
{
	_float3 vLook = *D3DXVec3Normalize(&vLook, &Get_State(STATE_LOOK));
	if (D3DXVec3Length(&vLook) == 0.f)
		vLook.z = 1.f;

	vLook *= fZ;

	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Add_Pos(const _float3& vAdd)
{
	_float3 vPos = Get_State(STATE_POSITION);

	vPos += vAdd;

	Set_State(STATE_POSITION, &vPos);
}

void CTransform::Set_Scale(const _float3& vScale)
{
	_float3 vRight = *D3DXVec3Normalize(&vRight, &Get_State(STATE_RIGHT)) * vScale.x;
	_float3 vUp = *D3DXVec3Normalize(&vUp, &Get_State(STATE_UP)) * vScale.y;
	_float3 vLook = *D3DXVec3Normalize(&vLook, &Get_State(STATE_LOOK)) * vScale.z;

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Set_Target(const _float3& vCurPoint, const _float3& vWorldPoint)
{
	_float3 vDir = vWorldPoint - vCurPoint;
	D3DXVec3Normalize(&vDir, &vDir);

	vDir *= Get_Scale().z;
	Set_State(STATE_LOOK, &vDir);
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC*		pDesc = (TRANSFORM_DESC*)pArg;

		m_fSpeedPerSec = pDesc->fSpeedPerSec;
		m_fRotationPerSec = pDesc->fRotationPerSec;
	}

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);		
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Jump(_float fTimeDelta, _float fJumpPower)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * fJumpPower * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Floor_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vLook = { vLook.x, 0.f, vLook.z };
	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Floor_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vLook = { vLook.x, 0.f, vLook.z };
	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Floor_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vRight = { vRight.x, 0.f, vRight.z };
	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Go_Floor_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vRight = { vRight.x, 0.f, vRight.z };
	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

void CTransform::Head_Roll(_float fTimeDelta, _float Degree)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix;
	
	D3DXMatrixRotationAxis(&RotationMatrix, &vLook, To_Radian(Degree));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Set_View_RollBack()
{
	//Set_State(STATE_POSITION, &m_vOffset_Position);
	m_WorldMatrix = m_WorldMatrix_Offset;
}

void CTransform::Set_HeadUp_Initialize()
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vRight, 0.f);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Camera_Shake(_float fTimeDelta, _float fShakePower)
{
	_float3		vLook = Get_State(STATE_LOOK);
	_float3		vRight = Get_State(STATE_RIGHT);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1000);

	int i = 0;
	if ( 0 >= (dis(gen) - 500) / 500.f) i = -1;
	else i = 1;

	Camera_Shake_Init();

	if (dis(gen) % 2 == 0) {
		Turn(Get_State(STATE_RIGHT), fShakePower * i);
		m_iShakeDirection = 1;
	}
	else {
		Turn(_float3(0.f, 1.f, 0.f), fShakePower * i);
		m_iShakeDirection = -1;
	}

	m_fShakePowerReverse = fShakePower * i;

	return;

}

void CTransform::Camera_Shake_Init()
{
	if(m_iShakeDirection == 1)
		Turn(Get_State(STATE_RIGHT), -m_fShakePowerReverse);
	else if(m_iShakeDirection == -1)
		Turn(_float3(0.f, 1.f, 0.f), -m_fShakePowerReverse);
}

void CTransform::Camera_Shake_End()
{
	Camera_Shake_Init();
	m_iShakeDirection = 0;
	m_fShakePowerReverse = 0.f;
}



void CTransform::Camera_Gun_Shake(_float fTimeDelta, _float fShakePower)
{
	_float3		vLook = Get_State(STATE_LOOK);
	_float3		vRight = Get_State(STATE_RIGHT);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1000);

	int i = 0;
	if (0 >= (dis(gen) - 500) / 500.f) i = -1;
	else i = 1;

	if (dis(gen) % 2 == 0) {
		Turn(Get_State(STATE_RIGHT), fShakePower * i);

	}
	else {
		Turn(_float3(0.f, 1.f, 0.f), fShakePower * i);
	}
	return;

}

void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	/* 세가지 방향벡터를 모두 회전시킨다. */
	/* 방향벡터의 회전 = 방향벡터 * 행렬 */
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::UnOffset_Turn(const _float3& vAxis, _float fTimeDelta)
{
	/* 세가지 방향벡터를 모두 회전시킨다. */
	/* 방향벡터의 회전 = 방향벡터 * 행렬 */
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_UnOffset_State(STATE_RIGHT, &vRight);
	Set_UnOffset_State(STATE_UP, &vUp);
	Set_UnOffset_State(STATE_LOOK, &vLook);
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scale();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Rotation_XYZ(const _float3& vXYZ)
{
	_float3		vScaled = Get_Scale();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4	Temp, RotationMatrix;
	D3DXMatrixIdentity(&RotationMatrix);

	RotationMatrix *= *D3DXMatrixRotationX(&Temp, To_Radian(vXYZ.x));
	RotationMatrix *= *D3DXMatrixRotationY(&Temp, To_Radian(vXYZ.y));
	RotationMatrix *= *D3DXMatrixRotationZ(&Temp, To_Radian(vXYZ.z));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::LookAt(const _float3& vWorldPoint)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vLook = vWorldPoint - vPosition;
	_float3		vRight = *D3DXVec3Cross(&vRight, &_float3(0.0f, 1.f, 0.f), &vLook);
	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	_float3		vScaled = Get_Scale();

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z;

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::LookAt_ForLandObject(const _float3& vWorldPoint)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vLook = vWorldPoint - vPosition;
	_float3		vRight = *D3DXVec3Cross(&vRight, &_float3(0.0f, 1.f, 0.f), &vLook);
	vLook = *D3DXVec3Cross(&vLook, &vRight, &Get_State(STATE_UP));

	_float3		vScaled = Get_Scale();

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z;

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Update_Offset()
{
	m_WorldMatrix_Offset = m_WorldMatrix;
	/*
	m_vOffset_Position = Get_State(STATE_POSITION);
	m_vOffset_Right = Get_State(STATE_RIGHT);
	m_vOffset_Up = Get_State(STATE_UP);
	m_vOffset_Look = Get_State(STATE_LOOK);
	*/
}

void CTransform::MonsterDieUi_Go_Up(_float fTimeDelta, _float Speed)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * Speed * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

// CGameObject*	p사본 = 원형->Clone();

CComponent * CTransform::Clone(void* pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTransform::Free()
{
	__super::Free();

}
