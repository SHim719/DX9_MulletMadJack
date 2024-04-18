#include "FPS_Camera.h"
#include "Core_Camera.h"
#include "GameObject.h"
#include "PlayerManager.h"
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


	//카메라 뷰 초기화 및 카메라 고정
	POINT ptWindow = { g_iWinSizeX >> 1, g_iWinSizeY >> 1 };
	ClientToScreen(g_hWnd, &ptWindow);
	SetCursorPos(ptWindow.x, ptWindow.y);

	// 빌보드 행렬을 단위행렬로 초기화
	D3DXMatrixIdentity(&m_BillboardMatrix);

	return S_OK;
}

void CFPS_Camera::PriorityTick(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CFPS_Camera::Tick(_float fTimeDelta)
{

	POINT		ptMouse;
	POINT		ptWindow = { g_iWinSizeX >> 1, g_iWinSizeY >> 1 };


	HWND		focus_hWnd = GetFocus();
	if (focus_hWnd != NULL && CPlayer_Manager::Get_Instance()->Get_MouseLock() == true) {

		//ShowCursor(FALSE);
		GetCursorPos(&ptMouse);

		ScreenToClient(g_hWnd, &ptMouse);

		_int	iMouseMove = { 0 };

		if (iMouseMove = ptMouse.x - ptWindow.x)
		{
			if (fabs(iMouseMove) > m_MoveSensitivity)
				m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * iMouseMove * m_CameraDesc.fMouseSensor);
		}

		if (iMouseMove = ptMouse.y - ptWindow.y)
		{
			if (fabs(iMouseMove) > m_MoveSensitivity && fabs(iMouseMove) < 400.f) {

				if (m_fVerticalAngle <= m_fVerticalAngleLimit && iMouseMove > 0.f) {
					m_fVerticalAngle += iMouseMove * m_CameraDesc.fMouseSensor;
					m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * iMouseMove * m_CameraDesc.fMouseSensor);
				} 
				else if (m_fVerticalAngle >= -m_fVerticalAngleLimit && iMouseMove < 0.f) {
					m_fVerticalAngle += iMouseMove * m_CameraDesc.fMouseSensor;
					m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * iMouseMove * m_CameraDesc.fMouseSensor);
				}
			}
		}
		ClientToScreen(g_hWnd, &ptWindow);

		SetCursorPos(ptWindow.x, ptWindow.y);

		_float fTempFovy = m_CameraDesc.fFovy;
		if (CPlayer_Manager::Get_Instance()->Get_Player_State() == CPlayer::PLAYER_STATE::DASH_STATE
			|| CPlayer_Manager::Get_Instance()->Get_Player_State() == CPlayer::PLAYER_STATE::AIRDASH_STATE
			|| CPlayer_Manager::Get_Instance()->Get_Player_State() == CPlayer::PLAYER_STATE::SLOPE_STATE) {
			fTempFovy = D3DXToRadian(80.0f);
		}



		D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, fTempFovy, g_iWinSizeX / (_float)g_iWinSizeY, m_CameraDesc.fNear, m_CameraDesc.fFar);
		m_ViewMatrix = m_pTransformCom->Get_WorldMatrix_Inverse();
	
		// 카메라 월드 행렬의 역행렬이 카메라의 view 행렬임
		// 카메라 view 행렬의 y축 회전 성분을 추려냄
		m_BillboardMatrix._11 = m_ViewMatrix._11;
		m_BillboardMatrix._13 = m_ViewMatrix._13;
		m_BillboardMatrix._31 = m_ViewMatrix._31;
		m_BillboardMatrix._33 = m_ViewMatrix._33;

		// y축 회전 성분이 담긴 행렬을 역변환하면 y축 빌보드 행렬이 됨
		D3DXMatrixInverse(&m_BillboardMatrix, nullptr, &m_BillboardMatrix);

	}

	

}

void CFPS_Camera::LateTick(_float fTimeDelta)
{

}

HRESULT CFPS_Camera::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
}

void CFPS_Camera::Mouse_Ray()
{
	_float3 fMouseNDC_Near = _float3(_float(g_iWinSizeX/2) * 2.0f / g_iWinSizeX - 1, -_float(g_iWinSizeY/2) * 2.0f / g_iWinSizeY + 1, 0.f);
	_float3 fMouseNDC_Far = _float3(_float(g_iWinSizeX/2) * 2.0f / g_iWinSizeX - 1, -_float(g_iWinSizeY/2) * 2.0f / g_iWinSizeY + 1, 1.f);

	_float4x4 inverseProjView;
	D3DXMatrixInverse(&inverseProjView, nullptr, &(m_ViewMatrix * m_ProjMatrix));

	_float3 fMouseWorld_Near = *D3DXVec3TransformCoord(&fMouseWorld_Near, &fMouseNDC_Near, &inverseProjView);
	_float3 fMouseWorld_Far = *D3DXVec3TransformCoord(&fMouseWorld_Far, &fMouseNDC_Far, &inverseProjView);

	_float3 vRayDir = *D3DXVec3Normalize(&vRayDir, &(fMouseWorld_Far - fMouseWorld_Near));

	RAY_DESC rayDesc{};
	rayDesc.iLevel = LEVEL_GAMEPLAY;
	rayDesc.strDstLayer = L"Enemy";
	rayDesc.vRayDir = vRayDir;
	rayDesc.vRayWorldPos = fMouseWorld_Near;

	m_pGameInstance->Add_RayDesc(rayDesc);
}

HRESULT CFPS_Camera::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &m_CameraDesc));

	return S_OK;
}

void CFPS_Camera::Key_Input(_float fTimeDelta)
{

	if (GetKeyState('Y') & 0x8000)
	{
		m_pTransformCom->Set_View_RollBack();
	}

	if (GetKeyState('U') & 0x8000)
	{
		m_pTransformCom->Debug_State_Out();
		m_pTransformCom->Debug_State_Out2();
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::RButton))
	{
		Mouse_Ray();
	}
}

CFPS_Camera* CFPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CFPS_Camera* pInstance = new CFPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
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
