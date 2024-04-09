#include "..\Public\Player.h"

#include "GameInstance.h"



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
	//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.5f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	CCoreCamera* pCamera = m_pGameInstance->Get_CurCamera();

	m_pTransformCom = pCamera->Get_Transform();
	Safe_AddRef(m_pTransformCom);

 	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
	//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);


	return S_OK;

}

void CPlayer::PriorityTick(_float fTimeDelta)
{
	HeadTiltReset(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{

	Key_Input(fTimeDelta);
	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Head_Roll(fTimeDelta, fHeadTilt);
	//cout << fHeadTilt << endl;
}

void CPlayer::LateTick(_float fTimeDelta)
{

}

HRESULT CPlayer::Render()
{
	return S_OK;
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Straight(fTimeDelta);
	}

	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Floor_Backward(fTimeDelta);
	}

	//Left
	if (m_pGameInstance->GetKey(eKeyCode::A))
	{
		m_pTransformCom->Go_Floor_Left(fTimeDelta);
		HeadTilt(fTimeDelta, -1.f);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::A))
	{
		//HeadTiltReset(fTimeDelta);
		//HeadTiltReset(fTimeDelta);
		fHeadTilt = 0.f;
	}

	if (m_pGameInstance->GetKey(eKeyCode::D))
	{
		m_pTransformCom->Go_Floor_Right(fTimeDelta);
		HeadTilt(fTimeDelta, 1.f);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::D))
	{
		//HeadTiltReset(fTimeDelta);
		//HeadTiltReset(fTimeDelta);
		fHeadTilt = 0.f;
	}

	if (GetKeyState('Z') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('X') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::Q))
	{
		m_pTransformCom->Set_View_RollBack();
		m_pTransformCom->Head_Roll(fTimeDelta, 320.f);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::E))
	{
		m_pTransformCom->Set_View_RollBack();
		m_pTransformCom->Head_Roll(fTimeDelta, -320.f);
	}

	if (GetKeyState('Y') & 0x8000)
	{
		m_pTransformCom->Set_HeadUp_Initialize();
	}
}

void CPlayer::HeadTilt(_float fTimeDelta, _float fDirection)
{
	if (fDirection > 0)
	{
		fHeadTilt = fMaxHeadTilt;
	}
	else
	{
		fHeadTilt = -fMaxHeadTilt;
	}
}

void CPlayer::HeadTiltReset(_float fTimeDelta)
{
	m_pTransformCom->Head_Roll(fTimeDelta, fHeadTilt * -1.f);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransformCom);
}
