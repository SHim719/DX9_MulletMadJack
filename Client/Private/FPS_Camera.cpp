#include "FPS_Camera.h"
#include "Core_Camera.h"
#include "GameObject.h"

CFPS_Camera::CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCoreCamera{ pGraphic_Device }
{

}

CFPS_Camera::CFPS_Camera(const CFPS_Camera& rhs)
	: CCoreCamera{ rhs }
{

}

HRESULT CFPS_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFPS_Camera::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof m_CameraDesc);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_CameraDesc.vEye);
	m_pTransformCom->LookAt(m_CameraDesc.vAt);

	POINT		ptMouse;

	GetCursorPos(&ptMouse);

	m_vOldMousePos = _float2((float)ptMouse.x, (float)ptMouse.y);

	return S_OK;
}

void CFPS_Camera::PriorityTick(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Straight(fTimeDelta);
	}

	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Backward(fTimeDelta);
	}

	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Left(fTimeDelta);
	}

	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Right(fTimeDelta);
	}

	if (GetKeyState('Z') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('X') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (GetKeyState('Q') & 0x8000)
	{
		m_pTransformCom->Head_Roll_Left(fTimeDelta, 60.f);
	}

	if (GetKeyState('E') & 0x8000)
	{
		m_pTransformCom->Head_Roll_Right(fTimeDelta, 60.f);
	}

	POINT		ptMouse;
	POINT		pt2 = { g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_int	iMouseMove = { 0 };

	if (iMouseMove = ptMouse.x - pt2.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * iMouseMove * m_CameraDesc.fMouseSensor);
	}

	if (iMouseMove = ptMouse.y - pt2.y)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * iMouseMove * m_CameraDesc.fMouseSensor);
	}

	ClientToScreen(g_hWnd, &pt2);

	SetCursorPos(pt2.x, pt2.y);



	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrix_Inverse());

	_float4x4		ProjMatrix;

	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, g_iWinSizeX / (_float)g_iWinSizeY, m_CameraDesc.fNear, m_CameraDesc.fFar));
}

void CFPS_Camera::Tick(_float fTimeDelta)
{

}

void CFPS_Camera::LateTick(_float fTimeDelta)
{

}

HRESULT CFPS_Camera::Render()
{

	return S_OK;
}

HRESULT CFPS_Camera::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Com_Transform"), &m_CameraDesc));

	return S_OK;
}

CFPS_Camera* CFPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFPS_Camera* pInstance = new CFPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFPS_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFPS_Camera::Clone(void* pArg)
{
	CFPS_Camera* pInstance = new CFPS_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CFPS_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFPS_Camera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
