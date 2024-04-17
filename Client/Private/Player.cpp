#include "..\Public\Player.h"
#include "GameInstance.h"
#include "PlayerManager.h"

#include "White_Suit_Monster.h"


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

	m_strTag = "Player";
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 0.3f, 2.5f, 0.3f };
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
	//LifeCut
	if(m_fPlayerHp > 0.f) m_fPlayerHp -= fTimeDelta;
	else {
		m_fPlayerHp = 0.f;
		//DeathAnimation Trigger
	}

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Head_Roll(fTimeDelta, fHeadTilt);
	//cout << fHeadTilt << endl;

	Render_Weapon();
	Render_Hand();

	Camera_Event(fTimeDelta);
	Jump_Tick(fTimeDelta);



}

void CPlayer::LateTick(_float fTimeDelta)
{
	ColliderCheck(fTimeDelta);
	SpeedControl(fTimeDelta);
	Shot();
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
		/*m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Kick"), true);*/
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Neck"), true);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Head"), true);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Body"), true);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Hand"), true);

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
		bDash = true;
	}

	if (m_pGameInstance->GetKeyUp(eKeyCode::LShift))
	{
		m_pTransformCom->Set_Speed(6.f);
		bDash = false;
		CPlayer_Manager::Get_Instance()->Set_Player_State(CPlayer::PLAYER_STATE::JUMP_STATE);
	}



	if (m_pGameInstance->GetKeyUp(eKeyCode::Space) && ePlayerState == IDLE_STATE)
	{
		Jump(10);
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

void CPlayer::Kick()
{
	Camera_Shake_Order(6000000.f, 0.4f);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Kick"), true);
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

	if (m_pGameInstance->GetKey(eKeyCode::LShift))
	{
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	}
}

void CPlayer::Jump(_float _fJumpPower)
{
	Set_PlayerState(CPlayer::PLAYER_STATE::JUMP_STATE);
	m_fJumpPower = _fJumpPower;
}

void CPlayer::Shot()
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
	{
		_float3 fMouseNDC_Near = _float3(_float(g_iWinSizeX / 2) * 2.0f / g_iWinSizeX - 1, -_float(g_iWinSizeY / 2) * 2.0f / g_iWinSizeY + 1, 0.f);
		_float3 fMouseNDC_Far = _float3(_float(g_iWinSizeX / 2) * 2.0f / g_iWinSizeX - 1, -_float(g_iWinSizeY / 2) * 2.0f / g_iWinSizeY + 1, 1.f);

		_float4x4 ViewMatrix, ProjMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

		_float4x4 inverseProjView;
		D3DXMatrixInverse(&inverseProjView, nullptr, &(ViewMatrix * ProjMatrix));

		_float3 fMouseWorld_Near = *D3DXVec3TransformCoord(&fMouseWorld_Near, &fMouseNDC_Near, &inverseProjView);
		_float3 fMouseWorld_Far = *D3DXVec3TransformCoord(&fMouseWorld_Far, &fMouseNDC_Far, &inverseProjView);

		_float3 vRayDir = *D3DXVec3Normalize(&vRayDir, &(fMouseWorld_Far - fMouseWorld_Near));

		RAY_DESC rayDesc{};
		rayDesc.iLevel = LEVEL_GAMEPLAY;
		rayDesc.strDstLayer = L"Monster";
		rayDesc.vRayDir = vRayDir;
		rayDesc.vRayWorldPos = fMouseWorld_Near;
		
		CPawn::ENEMYHIT_DESC pDesc;
		rayDesc.pArg = &pDesc;

		m_pGameInstance->Add_RayDesc(rayDesc);
	}
}

void CPlayer::SpeedControl(_float fTimeDelta)
{
	if (bDash)
	{
		_float RealTimeSpeed  = m_pTransformCom->Get_Speed();
		RealTimeSpeed -= fTimeDelta * 10.f;
		if(RealTimeSpeed <= 0.f) RealTimeSpeed = 0.f;
		m_pTransformCom->Set_Speed(RealTimeSpeed);
	}


}

void CPlayer::ColliderCheck(_float fTimeDelta)
{
	ColliderTop(fTimeDelta);
	ColliderLeft(fTimeDelta);
	ColliderRight(fTimeDelta);
	ColliderBack(fTimeDelta);
	ColliderFront(fTimeDelta);
}

void CPlayer::ColliderTop(_float fTimeDelta)
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Floor";
	rayDesc.vRayDir = { 0.f, 1.f, 0.f };
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.75f)
		{
			_float3 vPos = fHitWorldPos;
			vPos.y -= 0.75f;
			m_fJumpPower = -1.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::ColliderLeft(_float fTimeDelta)
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = { -1.f, 0.f, 0.f };
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::ColliderRight(_float fTimeDelta)
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = { 1.f, 0.f, 0.f };
	rayDesc.vRayDir = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::ColliderBack(_float fTimeDelta)
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = { 0.f, 0.f, -1.f };
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::ColliderFront(_float fTimeDelta)
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = { 0.f, 0.f, 1.f };
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::OnCollisionEnter(CGameObject* pOther)
{
	if ("Monster" == pOther->Get_Tag() && m_pGameInstance->GetKey(eKeyCode::LShift)) // 이거수정
	{
		static_cast<CWhite_Suit_Monster*>(pOther)->SetState_Pushed(m_pTransformCom->Get_Look());
		Kick();
	}
}

void CPlayer::Jump_Tick(_float fTimeDelta)
{
	if (ePlayerState == PLAYER_STATE::JUMP_STATE || ePlayerState == PLAYER_STATE::DASH_STATE)
	{
		m_pTransformCom->Go_Jump(m_fJumpPower, fTimeDelta);
		m_fJumpPower -= 19.8f * fTimeDelta;
	}

	if(ePlayerState == PLAYER_STATE::JUMP_STATE || ePlayerState == PLAYER_STATE::DASH_STATE)
	{
		RAY_DESC rayDesc;
		rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
		rayDesc.strDstLayer = L"Floor";
		rayDesc.vRayDir = { 0.f, -1.f, 0.f };
		rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		CGameObject* pHitObj = nullptr;
		_float3 fHitWorldPos;
		_float fDist = 0.f;

		if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
		{
			if (fDist <= 0.75f)
			{
				_float3 vPos = fHitWorldPos;
				vPos.y += 0.75f;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
				Set_PlayerState(CPlayer::PLAYER_STATE::IDLE_STATE);
			}
		}
	}



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
