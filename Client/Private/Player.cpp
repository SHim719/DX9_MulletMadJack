#include "..\Public\Player.h"
#include "GameInstance.h"
#include "PlayerManager.h"


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
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Phone"), true);
	//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);


	return S_OK;

}

void CPlayer::PriorityTick(_float fTimeDelta)
{
	HeadTiltReset(fTimeDelta);
	Key_Input(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Head_Roll(fTimeDelta, fHeadTilt);
	//cout << fHeadTilt << endl;

	Render_Weapon();
	Render_Hand();

	Camera_Event(fTimeDelta);




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
	if (m_pGameInstance->GetKey(eKeyCode::W))
	{
		Set_MoveState(MOVE);
		m_pTransformCom->Go_Floor_Straight(fTimeDelta);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::W))
	{
		Set_MoveState(STOP);
	}

	if (m_pGameInstance->GetKey(eKeyCode::S))
	{
		Set_MoveState(MOVE);
		m_pTransformCom->Go_Floor_Backward(fTimeDelta);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::S))
	{
		Set_MoveState(STOP);
	}

	//Left
	if (m_pGameInstance->GetKey(eKeyCode::A))
	{
		m_pTransformCom->Go_Floor_Left(fTimeDelta);
		Set_MoveState(MOVE);
		HeadTilt(fTimeDelta, -2.f);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::A))
	{
		Set_MoveState(STOP);
		fHeadTilt = 0.f;
	}

	if (m_pGameInstance->GetKey(eKeyCode::D))
	{
		m_pTransformCom->Go_Floor_Right(fTimeDelta);
		Set_MoveState(MOVE);
		HeadTilt(fTimeDelta, 2.f);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::D))
	{
		Set_MoveState(STOP);
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

	if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
	{
		Camera_Shake_Order(100000.f, 0.2f);
		for (int i=0; i < 6; i++) {	m_pGameInstance->Add_Ui_LifeClone(TEXT("CPistol_Gunfire"), eUiRenderType::Render_NonBlend, &i); }
		CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::SHOT);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::R))
	{
		CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::SPIN);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::B))
	{
		Camera_Shake_Order(600000.f, 0.4f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Kick"), true);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::LShift))
	{
		Camera_Shake_Order(200000.f, 0.2f);
		if (m_pGameInstance->GetKey(eKeyCode::W)){
			m_pTransformCom->Set_Speed(15.f);
			m_pTransformCom->Go_Floor_Straight(fTimeDelta);
			m_pTransformCom->Go_Floor_Straight(fTimeDelta);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::S)) {
			m_pTransformCom->Set_Speed(15.f);
			m_pTransformCom->Go_Floor_Backward(fTimeDelta);
			m_pTransformCom->Go_Floor_Backward(fTimeDelta);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::A)) {
			m_pTransformCom->Set_Speed(15.f);
			m_pTransformCom->Go_Floor_Left(fTimeDelta);
			m_pTransformCom->Go_Floor_Left(fTimeDelta);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::D)) {
			m_pTransformCom->Set_Speed(15.f);
			m_pTransformCom->Go_Floor_Right(fTimeDelta);
			m_pTransformCom->Go_Floor_Right(fTimeDelta);
		}
		else {
			m_pTransformCom->Set_Speed(15.f);
			m_pTransformCom->Go_Floor_Straight(fTimeDelta);
			m_pTransformCom->Go_Floor_Straight(fTimeDelta);
		}

		m_pTransformCom->Set_Speed(10.f);
		CPlayer_Manager::Get_Instance()->Set_Player_State(CPlayer::PLAYER_STATE::DASH_STATE);
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::LShift))
	{
		m_pTransformCom->Set_Speed(6.f);
		CPlayer_Manager::Get_Instance()->Set_Player_State(CPlayer::PLAYER_STATE::IDLE_STATE);
	}




	if (m_pGameInstance->GetKeyUp(eKeyCode::Space))
	{
		Jump(fTimeDelta);
	}
	if (GetKeyState('T') & 0x8000)
	{
		Camera_Shake_Order(100.f, 0.5f);
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

void CPlayer::Render_Weapon()
{
	if (eWeaponType == PISTOL && eHandType == IDLE_HAND && eAnimationType == IDLE)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
		return;
	}

	if (eWeaponType == PISTOL && eHandType == IDLE_HAND && eAnimationType == SHOT)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);
		return;
	}

	if (eWeaponType == PISTOL && eHandType == IDLE_HAND && eAnimationType == SPIN)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Spin"), true);
		return;
	}

	if (eWeaponType == PISTOL && eHandType == IDLE_HAND && eAnimationType == RELOAD)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Reload"), true);
		return;
	}


	MSG_BOX(TEXT("What????????????"));

}

void CPlayer::Render_Hand()
{
	if (eWeaponType == PISTOL && eHandType == IDLE_HAND && eAnimationType == IDLE)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
	}
}

void CPlayer::Active_Reset()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Spin"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Reload"), false);
}

void CPlayer::Camera_Reset()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), false);
}

void CPlayer::Move_Reset()
{
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
void  CPlayer::Camera_Event(_float fTimeDelta)
{
	if (m_fShakeTime >= 0.f) Camera_Shake(fTimeDelta, m_fShakePower, m_fShakeTime);

	if (ePlayerState == PLAYER_STATE::DASH_STATE)
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	}
}

void CPlayer::Jump(_float fTimeDelta)
{
	Set_PlayerState(CPlayer::PLAYER_STATE::JUMP_STATE);
}

void CPlayer::Camera_Shake(_float fTimeDelta, _float fShakePower, _float& fShakeTime)
{
	if (fShakeTime > 0.f)
	{
		m_pTransformCom->Camera_Shake(fTimeDelta, fShakePower);

		fShakeTime -= fTimeDelta;
		fShakePower = fShakePower * 0.9f;
		if (fShakeTime < 0.f) {
			m_pTransformCom->Camera_Shake_End();
			fShakeTime = 0.f;
			fShakePower = 0.f;
		}
	}

	return;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransformCom);
}
