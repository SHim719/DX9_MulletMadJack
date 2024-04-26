#include "Player.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "CSans_Gaster.h"
#include "Pawn.h"

#include "Ui_Include.h"
#include "CUi_Sans_Heart.h"
#include "DialogueManager.h"




CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject{rhs}
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
	desc.vScale = { 0.8f, 1.2f, 0.8f };
	desc.vOffset = { 0.f, -0.2f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	CCoreCamera* pCamera = m_pGameInstance->Get_CurCamera();

	m_pTransformCom = pCamera->Get_Transform();
	Safe_AddRef(m_pTransformCom);

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));
	if (nullptr == m_pRigidbody)
		return E_FAIL;

 	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Phone"), true);
	//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);

	m_strTag = "Player";
	return S_OK;

}

void CPlayer::PriorityTick(_float fTimeDelta)
{
	HeadTiltReset(fTimeDelta);
	if (CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_NONE) Key_Input(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	Process_State(fTimeDelta);
	Reload_Warning();
	//LifeCut
	if (CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_NONE) {
		if (m_fPlayerHp > 0.f) m_fPlayerHp -= fTimeDelta;
		else {
			m_fPlayerHp = 0.f;
			//DeathAnimation Trigger
		}
	}

	//Invincible
	if (Get_InvincibleTime() > 0.f) { 
		Set_InvincibleTime(Get_InvincibleTime() - fTimeDelta);
		Set_Invincible(true);
	}
	else if (Get_InvincibleTime() > -100.f)
	{ 
		Set_InvincibleTime(0.f);
		Set_Invincible(false);
	}


	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Head_Roll(fTimeDelta, fHeadTilt);
	//cout << fHeadTilt << endl;

	Render_Weapon();

	Camera_Event(fTimeDelta);

	m_pRigidbody->Update(fTimeDelta);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	Execution_Alert();
	//if(m_bHaveWeapon == true) 
	//	m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"), true);
	//else 
	//	m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"), false);


	ColliderCheck();
	Shot();
}

HRESULT CPlayer::Render()
{
	
	return S_OK;
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_pGameInstance->GetKey(eKeyCode::One)) {
		Set_TimeDivide(1.f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_BulletTime"), false);
	}
	if (m_pGameInstance->GetKey(eKeyCode::Two)) {
		Set_TimeDivide(2.f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_BulletTime"), true);
	}
	if (m_pGameInstance->GetKey(eKeyCode::Three)) {
		Set_TimeDivide(3.f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_BulletTime"), true);
	}
	if (m_pGameInstance->GetKey(eKeyCode::Four)) {
		Set_TimeDivide(4.f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_BulletTime"), true);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::M))
	{
		CDialogue_Manager::Get_Instance()->Start_Dialogue(DialogueEvent::FirstDialogue);
	}

	if (GetKeyState('Z') & 0x8000)
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('X') & 0x8000)
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

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

	if (m_pGameInstance->GetKeyDown(eKeyCode::LButton) && eAnimationType != SPIN)
	{
		if (eAnimationType == IDLE || eWeaponType == PISTOL) {
			if (CPlayer_Manager::Get_Instance()->Get_Magazine() > 0) {
				Attack();
				CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(SHOT);
				CPlayer_Manager::Get_Instance()->Fire_Magazine();
			}
			else {
				if (eWeaponType == PISTOL && eAnimationType == IDLE) {
					m_pGameInstance->Play(L"Pistol_Reload", false);
					m_pGameInstance->SetVolume(L"Pistol_Reload", 1.f);

					CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(SPIN);
				}
				else if (eWeaponType == SHOTGUN && eAnimationType != RELOAD) {
					m_pGameInstance->Play(L"Shotgun_Reload", false);
					m_pGameInstance->SetVolume(L"Shotgun_Reload", 1.f);

					CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(RELOAD);
				}
				else if(eAnimationType != RELOAD){
					CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(RELOAD);
				}
			}
		}
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::R) && eAnimationType == IDLE)
	{
		if (eWeaponType == PISTOL) {
			m_pGameInstance->Play(L"Pistol_Reload", false);
			m_pGameInstance->SetVolume(L"Pistol_Reload", 1.f);

			CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::SPIN);
		}
		else {
			m_pGameInstance->Play(L"Shotgun_Reload", false);
			m_pGameInstance->SetVolume(L"Shotgun_Reload", 1.f);

			CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::RELOAD);
		}
	}
#pragma region TestCode
	if (m_pGameInstance->GetKeyDown(eKeyCode::N))
	{
		m_pGameInstance->Play(L"Player_Weapon_Change", false);
		m_pGameInstance->SetVolume(L"Player_Weapon_Change", 1.f);

		if(eWeaponType == PISTOL) CPlayer_Manager::Get_Instance()->WeaponChange(SHOTGUN);
		else if (eWeaponType == SHOTGUN) CPlayer_Manager::Get_Instance()->WeaponChange(KATANA);
		else CPlayer_Manager::Get_Instance()->WeaponChange(PISTOL);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::B))
	{
		m_pTransformCom->Set_Position({ 0.f, 3.f, 0.f });



		m_bHaveWeapon = true;
		//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Opening"), true);
	}
#pragma endregion
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
	switch (eWeaponType)
	{
		case PISTOL:
			Render_Pistol();
			break;
		case SHOTGUN :
			Render_Shotgun();
			break;
		case KATANA:
			Render_Katana();
			break;
		default:
			MSG_BOX(TEXT("What????????????"));
			break;
	}

}

void CPlayer::Reload_Warning()
{
	if (CPlayer_Manager::Get_Instance()->Get_Magazine() <= 2)	m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Reload"), true);
	else														m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Reload"), false);

	if (CPlayer_Manager::Get_Instance()->Get_Player_AnimationType() == OPENING || 
		CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_EXECUTION ||
		CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_CUTIN_SHOP) 
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Reload"), false);
}

void CPlayer::Execution_Alert()
{
	if (m_bHaveWeapon == true) m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"), true);
	else m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"), false);

	if (CPlayer_Manager::Get_Instance()->Get_Player_AnimationType() == OPENING ||
		CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_EXECUTION ||
		CPlayer_Manager::Get_Instance()->Get_Action_Type() == CPlayer_Manager::ACTION_CUTIN_SHOP)
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Execution_Show"), false);
}

void CPlayer::Render_Pistol()
{
	switch (eAnimationType)
	{
		case IDLE:
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
			return;	
		case SHOT:
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), true);
			return;
		case SPIN:
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Spin"), true);
			return;
		case RELOAD:
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Reload"), true);
			return;
		case OPENING:
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Opening"), true);
			return;
		default:
			return;
	}
}

void CPlayer::Render_Shotgun()
{
	switch (eAnimationType)
	{
	case IDLE:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun"), true);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
		return;
	case SHOT:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Shot"), true);
		return;
	case SPIN: {
		srand((unsigned int)time(NULL));
		int i = rand() % 2;
		if (i == 0)
		{
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_SpinA"), true);
		}
		else
		{
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_SpinB"), true);
		}
		return;
	}
	case OPENING:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Opening"), true); 
		return;
	case RELOAD:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Reload"), true);
		return;
	default:

		return;
	}
}

void CPlayer::Render_Katana()
{

	switch (eAnimationType)
	{
	case IDLE:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana"), true);
		//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
		return;
	case SHOT:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Slash"), true);
		return;
	case SPIN:
		//Temp
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana"), true);
		return;
	case OPENING:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Opening"), true);
		return;
	case RELOAD:
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Reload"), true);
		return;
	default:
		return;
	}
}

void CPlayer::Attack()
{
	switch (eWeaponType)
	{
	case PISTOL:
		Fire_Pistol();
		return;
	case SHOTGUN: 
		Fire_Shotgun();
		return;
	case KATANA:
		Slash_Katana();
		return;
	default: 
	
		return;
	}
}

void CPlayer::Fire_Pistol()
{
	Camera_Shake_Order(100000.f, 0.2f);
	for (int i = 0; i < 6; i++) { m_pGameInstance->Add_Ui_LifeClone(TEXT("CPistol_Gunfire"), eUiRenderType::Render_NonBlend, &i); }

	m_pGameInstance->Play(L"Pistol_Fire", false);
	m_pGameInstance->SetVolume(L"Pistol_Fire", 1.f);
}

void CPlayer::Fire_Shotgun() {
	Camera_Shake_Order(800000.f, 0.4f);
	for (int i = 0; i < 12; i++) { m_pGameInstance->Add_Ui_LifeClone(TEXT("CShotgun_Gunfire"), eUiRenderType::Render_NonBlend, &i); }

	m_pGameInstance->Play(L"Shotgun_Fire", false);
	m_pGameInstance->SetVolume(L"Shotgun_Fire", 1.f);
}

void CPlayer::Slash_Katana(){
	Camera_Shake_Order(600000.f, 0.4f);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Effect"), true);

	m_pGameInstance->Play(L"Katana_Slash", false);
	m_pGameInstance->SetVolume(L"Katana_Slash", 1.f);
}

void CPlayer::Active_Reset()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Shot"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Spin"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Reload"), false);
	
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Shot"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_SpinA"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_SpinB"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Shotgun_Reload"), false);

	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Slash"), false);
	//m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Effect"), false);
}

void CPlayer::Camera_Reset()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), false);
}


void CPlayer::Kick()
{
	m_pGameInstance->Play(L"Player_Kick_Hit", false);
	m_pGameInstance->SetVolume(L"Player_Kick_Hit", 1.f);

	Camera_Shake_Order(2500000.f, 0.2f);
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



void CPlayer::Camera_Event(_float fTimeDelta)
{
	if (m_fShakeTime >= 0.f) Camera_Shake(fTimeDelta, m_fShakePower, m_fShakeTime);
}

void CPlayer::Shot()
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
	{
		if (OPENING == eAnimationType || RELOAD == eAnimationType)
			return;

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
		rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
		
		rayDesc.vRayWorldPos = fMouseWorld_Near;
		rayDesc.vRayDir = vRayDir;

		CPawn::ENEMYHIT_DESC pDesc;
		rayDesc.pArg = &pDesc;
		if (eWeaponType == SHOTGUN || eWeaponType == KATANA) {

			rayDesc.strDstLayer = L"Monster";
			m_pGameInstance->Add_RayDesc(rayDesc);

			rayDesc.strDstLayer = L"SlopeMonster";
			m_pGameInstance->Add_RayDesc(rayDesc);

			for (int i{ -3 }; i <= 3; ++i) {
				for (int j{ -3 }; j <= 3; ++j) {
					if (i == 0 && j == 0)
						continue;
					_float3 TempRayDir = vRayDir + _float3(i * 0.1f, j * 0.1f, 0.f);
					
					rayDesc.vRayDir = TempRayDir;
					rayDesc.strDstLayer = L"Monster";
					m_pGameInstance->Add_RayDesc(rayDesc);

					rayDesc.strDstLayer = L"SlopeMonster";
					m_pGameInstance->Add_RayDesc(rayDesc);
				}
			}
		}
		else {
			
			rayDesc.strDstLayer = L"Monster";
			m_pGameInstance->Add_RayDesc(rayDesc);

			rayDesc.strDstLayer = L"SlopeMonster";
			m_pGameInstance->Add_RayDesc(rayDesc);
		}
	}
}


void CPlayer::ColliderCheck()
{
	ColliderUpDown();
}

void CPlayer::ColliderUpDown()
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = m_pGameInstance->Get_CurrentLevelID();
	rayDesc.strDstLayer = L"Floor";
	rayDesc.vRayDir = { 0.f, -1.f, 0.f };

	_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	rayDesc.vRayWorldPos = vPlayerPos;

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	//&& m_pRigidbody->Get_Velocity().y < 0.f
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist) && fDist <= 1.0f)
	{
		_float3 vPos = fHitWorldPos;
		vPos.y += 1.0f;
		m_pTransformCom->Set_Position(vPos);

		if (m_bJumped)
		{
			m_pGameInstance->Play(L"Player_Land", false);
			m_pGameInstance->SetVolume(L"Player_Land", 1.f);

			m_bJumped = false;
		}

		m_pRigidbody->Set_OnGround();
		m_bCanAirDash = true;

		if (SLOPE_STATE == ePlayerState) {
			m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
			SetState_Idle();
		}

		return;
	} 
	else
	{
		m_pRigidbody->Set_Ground(false);
	}

	rayDesc.strDstLayer = L"Slope";
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist) && fDist <= 1.f)
	{
		_float3 vPos = fHitWorldPos;
		vPos.y += 1.f;
		m_pTransformCom->Set_Position(vPos);

		m_pSlopeTransform = pHitObj->Get_Transform();
		m_pRigidbody->Set_OnGround();
		SetState_Slope();
		m_bCanAirDash = true;
		return;
	}
	else if (SLOPE_STATE == ePlayerState)
	{
		SetState_Idle();
	}

	rayDesc.vRayDir = { 0.f, 1.f, 0.f };
	rayDesc.vRayWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	rayDesc.strDstLayer = L"Floor";
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.2f)
		{
			_float3 vPos = fHitWorldPos;
			vPos.y -= 0.2f;
			m_pRigidbody->Set_VelocityY(-0.5f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::OnCollisionEnter(CGameObject* pOther)
{
	if ("Monster" == pOther->Get_Tag() && (DASH_STATE == ePlayerState || AIRDASH_STATE == ePlayerState))
	{
		if (static_cast<CPawn*>(pOther)->Is_DeathState())
			return;

		if (m_bHaveWeapon)
		{
			if (static_cast<CPawn*>(pOther)->SetState_Execution())
			{
				static_cast<CPawn*>(pOther)->Set_Execution_Target();
				SetState_Execution();
				CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_EXECUTION);
				m_pExecutionEnemy = static_cast<CPawn*>(pOther);
			}
		}
		else
		{
			static_cast<CPawn*>(pOther)->SetState_Pushed(m_pTransformCom->Get_Look());
			Kick();
		}
		
	}

}

void CPlayer::Hit(void* pArg)
{
	_float* pDamage = (_float*)pArg;
	if (false == m_bInvincible && m_fPlayerHp > 0.f) {
		Set_PlayerHP_Add(-*pDamage);
		Set_InvincibleTime(Get_InvincibleTimeLimit());
		m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_Damaged"));

		m_pGameInstance->Play(L"Player_Damaged", false);
		m_pGameInstance->SetVolume(L"Player_Damaged", 0.3f);
	}
}

void CPlayer::SansHit()
{
	auto iter = m_SansHeartVec.begin();
	if (iter != m_SansHeartVec.end())
	{
		(*iter)->Set_Dead();
		m_SansHeartVec.erase(iter);
	}

}

void CPlayer::SansLevelEnterInitialize()
{
	CUi_Sans_Heart* Heart = nullptr;
	m_SansHeartVec.reserve(5);
	_float3 Pos = {600, 200, 0};
	for (size_t i = 0; i < 5; ++i)
	{
		Pos.x -= 50;
		Heart = (CUi_Sans_Heart*)(m_pGameInstance->
			Add_Ui_PartCloneRender(L"CUi_Sans_Heart", 
				eUiRenderType::Render_NonBlend, Pos));
		m_SansHeartVec.emplace_back(Heart);
	
	}

	Set_HpLimit(99);
	Set_PlayerHP(99);
}

void CPlayer::SansLevelExitInitialize()
{
	for (size_t i = 0; i < m_SansHeartVec.size(); ++i)
	{
		Safe_Release(m_SansHeartVec[i]);
	}
	m_SansHeartVec.clear();
	Set_HpLimit(15);
	Set_PlayerHP(15);
}



void CPlayer::Process_State(_float fTimeDelta)
{
	switch (ePlayerState)
	{
	case IDLE_STATE:
		Idle_State(fTimeDelta);
		break;
	case DASH_STATE:
		Dash_State(fTimeDelta);
		break;
	case AIRDASH_STATE:
		AirDash_State(fTimeDelta);
		break;
	case SLOPE_STATE:
		Slope_State(fTimeDelta);
		break;
	case EXECUTION_STATE:
		Execution_State();
		break;
	}
}

void CPlayer::Idle_State(_float fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
	m_pRigidbody->Set_GroundVelocity(_float3(0.f, 0.f, 0.f));

	if (m_pGameInstance->GetKey(eKeyCode::W))
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

		Set_MoveState(MOVE);
		m_pRigidbody->Set_GroundVelocity(m_pTransformCom->Get_GroundLook() * m_fMoveSpeed);
	}
	else if (m_pGameInstance->GetKeyUp(eKeyCode::W))
	{
		Set_MoveState(STOP);
	}

	if (m_pGameInstance->GetKey(eKeyCode::S))
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

		Set_MoveState(MOVE);
		m_pRigidbody->Set_GroundVelocity(-m_pTransformCom->Get_GroundLook() * m_fMoveSpeed);
	}
	else if (m_pGameInstance->GetKeyUp(eKeyCode::S))
	{
		Set_MoveState(STOP);
	}

	if (m_pGameInstance->GetKey(eKeyCode::A))
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

		m_pRigidbody->Add_GroundVelocity(-m_pTransformCom->Get_GroundRight() * m_fMoveSpeed);
		Set_MoveState(MOVE);
		HeadTilt(fTimeDelta, -2.f);
	}
	else if (m_pGameInstance->GetKeyUp(eKeyCode::A))
	{
		Set_MoveState(STOP);
		fHeadTilt = 0.f;
	}

	if (m_pGameInstance->GetKey(eKeyCode::D))
	{
		if (m_fSoundTime >= 1.f)
		{
			m_pGameInstance->Play(L"Player_Footstep", false);
			m_pGameInstance->SetVolume(L"Player_Footstep", 0.3f);

			m_fSoundTime = 0.f;
		}

		m_pRigidbody->Add_GroundVelocity(m_pTransformCom->Get_GroundRight() * m_fMoveSpeed);
		Set_MoveState(MOVE);
		HeadTilt(fTimeDelta, 2.f);
	}
	else if (m_pGameInstance->GetKeyUp(eKeyCode::D))
	{
		Set_MoveState(STOP);
		fHeadTilt = 0.f;
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::Space) && m_pRigidbody->IsGround())
	{
		m_pGameInstance->Play(L"Player_Jump", false);
		m_pGameInstance->SetVolume(L"Player_Jump", 1.f);

		m_bJumped = true;

		m_pRigidbody->Set_VelocityY(5.f);
		m_pRigidbody->Set_Ground(false);
	}
	
	if (m_pGameInstance->GetKeyDown(eKeyCode::LShift) && false == m_pRigidbody->IsGround() && m_bCanAirDash)
	{
		SetState_AirDash(); 
	}
	else if (m_pGameInstance->GetKey(eKeyCode::LShift) && true == m_pRigidbody->IsGround())
	{
		SetState_Dash();
	}
}

void CPlayer::Dash_State(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyUp(eKeyCode::LShift))
	{
		SetState_Idle();
		return;
	}

	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
	if (m_pGameInstance->GetKey(eKeyCode::D))
		HeadTilt(fTimeDelta, 2.f);
	else if (m_pGameInstance->GetKey(eKeyCode::A))
		HeadTilt(fTimeDelta, -2.f);
}

void CPlayer::AirDash_State(_float fTimeDelta)
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
	m_fDashTimeAcc += fTimeDelta;
	if (m_fDashTimeAcc >= m_fDashTime)
	{
		m_fDashTimeAcc = 0.f;
		SetState_Idle();
	}
}

void CPlayer::Slope_State(_float fTimeDelta)
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), true);
	if (m_pGameInstance->GetKey(eKeyCode::W))
	{
		_float3 vLook = m_pSlopeTransform->Get_Look();
		vLook.x = -vLook.x;
		vLook.z = -vLook.z;
		vLook *= 15.f;
		m_pRigidbody->Set_Velocity(vLook);
	}
	if (m_pGameInstance->GetKey(eKeyCode::D))
	{
		_float3 vRight = m_pSlopeTransform->Get_Right();
		vRight *= 5.f;
		m_pRigidbody->Add_GroundVelocity(vRight);
	}
	else if (m_pGameInstance->GetKey(eKeyCode::A))
	{
		_float3 vRight = m_pSlopeTransform->Get_Right();
		D3DXVec3Normalize(&vRight, &vRight);
		vRight *= 5.f;
		m_pRigidbody->Add_GroundVelocity(-vRight);
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::Space) && m_pRigidbody->IsGround())
	{
		m_pRigidbody->Set_VelocityY(5.f);
		m_pRigidbody->Set_Ground(false);
	}
}

void CPlayer::Execution_State()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
	if (CPlayer_Manager::ACTION_NONE == CPlayer_Manager::Get_Instance()->Get_Action_Type())
	{
		Set_Invincible(false);
		Set_InvincibleTime(1.5f);
		SetState_Idle();
		Kick();
		if (m_pExecutionEnemy)
		{
			m_pExecutionEnemy->SetState_Fly(m_pTransformCom->Get_GroundLook());
			m_pExecutionEnemy = nullptr;
		}
	}
	
}

void CPlayer::SetState_Idle()
{
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), false);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Slide"), false);
	m_pRigidbody->Set_UseGravity(true);
	m_pRigidbody->Set_Friction(0.f);
	m_pRigidbody->Set_GroundVelocity(_float3(0.f, 0.f, 0.f));
	ePlayerState = IDLE_STATE;
	Set_MoveState(STOP);
}

void CPlayer::SetState_Dash()
{
	ePlayerState = DASH_STATE;
	m_pGameInstance->Play(L"Player_Dash", false);
	m_pGameInstance->SetVolume(L"Player_Dash", 1.f);
	
	if (m_pGameInstance->GetKey(eKeyCode::A))
		m_pRigidbody->Set_GroundVelocity(-m_pTransformCom->Get_GroundRight() * 15.f);
	else if (m_pGameInstance->GetKey(eKeyCode::D))
		m_pRigidbody->Set_GroundVelocity(m_pTransformCom->Get_GroundRight() * 15.f);
	else if (m_pGameInstance->GetKey(eKeyCode::S))
		m_pRigidbody->Set_GroundVelocity(-m_pTransformCom->Get_GroundLook() * 15.f);
	else
		m_pRigidbody->Set_GroundVelocity(m_pTransformCom->Get_GroundLook() * 15.f);
	m_pRigidbody->Set_Friction(1.8f);

}

void CPlayer::SetState_AirDash()
{
	ePlayerState = AIRDASH_STATE;
	m_pGameInstance->Play(L"Player_AirDash", false);
	m_pGameInstance->SetVolume(L"Player_AirDash", 1.f);

	if (m_pGameInstance->GetKey(eKeyCode::A))
		m_pRigidbody->Set_GroundVelocity(-m_pTransformCom->Get_GroundRight() * 15.f);
	else if (m_pGameInstance->GetKey(eKeyCode::D))
		m_pRigidbody->Set_GroundVelocity(m_pTransformCom->Get_GroundRight() * 15.f);
	else if (m_pGameInstance->GetKey(eKeyCode::S))
		m_pRigidbody->Set_GroundVelocity(-m_pTransformCom->Get_GroundLook() * 15.f);
	else
		m_pRigidbody->Set_GroundVelocity(m_pTransformCom->Get_GroundLook() * 15.f);

	m_pRigidbody->Set_UseGravity(false);
	m_pRigidbody->Set_VelocityY(0.f);

	m_bCanAirDash = false;
}

void CPlayer::SetState_Slope()
{
	ePlayerState = SLOPE_STATE;

	_float3 vSlopeLook = m_pSlopeTransform->Get_Look();
	vSlopeLook.x = -vSlopeLook.x;
	vSlopeLook.z = -vSlopeLook.z;
	m_pRigidbody->Set_Velocity(vSlopeLook * 5.f);

	m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);

	m_pRigidbody->Set_UseGravity(true);
	m_pRigidbody->Set_Friction(0.f);

	Set_MoveState(STOP);
}

void CPlayer::SetState_Execution()
{
	ePlayerState = EXECUTION_STATE;

	CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_EXECUTION);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pRigidbody->Set_UseGravity(false);
	m_pRigidbody->Set_Friction(0.f);

	m_bHaveWeapon = false;

	Set_Invincible(true);
	Set_InvincibleTime(-150.f);
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

	for (size_t i = 0; i < m_SansHeartVec.size(); ++i)
	{
		m_SansHeartVec[i]->Set_Dead();
	}
	m_SansHeartVec.clear();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRigidbody);
	Safe_Release(m_pTransformCom);
}
