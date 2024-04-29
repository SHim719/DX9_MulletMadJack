#include "Level_Boss.h"
#include "GameInstance.h"
#include "CGame_Manager.h"

#include "FPS_Camera.h"
#include "PlayerManager.h"
#include "Player.h"

#include "MapLoader.h"
#include "MapObject_Header.h"
#include "Trigger_Headers.h"

#include "Beholder.h"
#include "Artemis.h"
#include "CUi_BossHpBar.h"

CLevel_Boss::CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Boss::Initialize()
{
	m_iLevelID = LEVEL_BOSS;

	m_pGameInstance->Stop(L"Loading");

	m_pGameInstance->Play(L"Boss_Level_Start_BGM", true);
	m_pGameInstance->SetVolume(L"Boss_Level_Start_BGM", 0.3f);

	CGame_Manager::Get_Instance()->Set_StageProgress(StageProgress::OnGoing);

	Ready_Layer_Camera(TEXT("Main_Camera"));
	
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Beholder(L"Monster")))
		return E_FAIL;

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/TestMh.dat", (LEVEL)m_iLevelID);
	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
	CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer_Manager::Get_Instance()->Get_NextWeapon());

	m_pFPS_Camera = static_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());

	m_pFPS_Camera->Set_VerticalAngle(0.f);

	D3DLIGHT9 lightDesc{};
	lightDesc.Type = D3DLIGHT_DIRECTIONAL;
	lightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightDesc.Ambient = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
	lightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(m_pGraphic_Device->SetLight(9, &lightDesc)))
		return E_FAIL;

	D3DMATERIAL9		MaterialDesc{};
	MaterialDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MaterialDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);

	m_pGraphic_Device->SetMaterial(&MaterialDesc);

	m_pGraphic_Device->LightEnable(9, TRUE);

	if (nullptr == m_pGameInstance->Add_Clone(m_iLevelID, L"SkyBox", TEXT("Prototype_SkyBox")))
		return E_FAIL;

	static_cast<CBossEntryTrigger*>(m_pGameInstance->Find_GameObject(m_iLevelID, L"Trigger", 0))
		->Set_Level(this);

	return S_OK;
}

void CLevel_Boss::Tick(_float fTimeDelta)
{
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);

	switch (m_eState)
	{
	case NoEntry:

		break;
	case Entry:
		Entry_State(fTimeDelta);
		break;
	}
}

HRESULT CLevel_Boss::Render()
{
	return S_OK;
}


CLevel_Boss* CLevel_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Boss* pInstance = new CLevel_Boss(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CCoreCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 0.65f, 1.7f);
	CameraDesc.vAt = _float3(0.5f, 0.5f, 10.f);
	CameraDesc.fFovy = D3DXToRadian(90.0f);
	CameraDesc.fNear = 0.001f;
	CameraDesc.fFar = 100.0f;
	CameraDesc.fSpeedPerSec = 6.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Create_Camera(strLayerTag, CFPS_Camera::Create(m_pGraphic_Device, &CameraDesc))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Player()
{
	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(m_iLevelID, TEXT("Player"), TEXT("Prototype_Player")));
	CPlayer_Manager::Get_Instance()->Set_Player(m_pPlayer);

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	m_pPlayer->Set_PlayerHP(99.f);
	m_pPlayer->Set_PlayerHPMax(99.f);
	//m_pPlayer->Get_Transform()->Set_Pos({ 0.f, 1.1f, 0.f });

	m_pPlayer->Get_Transform()->Set_Pos({ 0.f, 52.f, -102.4f }); 
	m_pPlayer->Set_Weapon_Have(false);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Beholder(const wstring& strLayerTag)
{
	m_pBeholder = dynamic_cast<CBeholder*>
		(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag, TEXT("Prototype_Beholder")));
	static_cast<CBoxCollider*>(m_pBeholder->Find_Component(L"Collider"))->Update_BoxCollider(m_pBeholder->Get_Transform()->Get_WorldMatrix());

	m_pBeholder->Set_Active(false);

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_BossHpBar"),
		eUiRenderType::Render_Blend,
		CUi_BossHpBar::Create(m_pGraphic_Device))))
		return E_FAIL;

	CUi_BossHpBar* pHpBar = dynamic_cast<CUi_BossHpBar*>
		(m_pGameInstance->Get_ActiveBlendUI(L"CUi_BossHpBar"));
	if (pHpBar == nullptr)
		assert(false);

	pHpBar->Set_Beholder(m_pBeholder);

	///*if (nullptr == m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
	//	TEXT("Prototype_Artemis")))
	//	return E_FAIL;

	//if (nullptr == m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
	//	TEXT("Prototype_Apollo")))
	//	return E_FAIL;

	return S_OK;
}
void CLevel_Boss::SetState_Entry()
{
	m_eState = Entry;
	m_vEntryPos = m_pPlayer->Get_Transform()->Get_Pos();

	m_pFPS_Camera->Set_CutScene(true);
	m_pFPS_Camera->Get_Transform()->Set_State(CTransform::STATE_LOOK, &_float3(-0.2f, 0.3f, 0.95f));
	m_pPlayer->Set_CutScene(true);

	m_pBeholder->Set_Active(true);
	m_pBeholder->Set_CutScene(true);
}

void CLevel_Boss::Entry_State(_float fTimeDelta)
{
	_float fNowFovY = m_pFPS_Camera->Get_FovY();
	if (!m_bAnotherBranch)
	{
		if (fNowFovY > 45.f)
		{
			fNowFovY -= 20.f * fTimeDelta;
			if (fNowFovY <= 45.f)
			{
				fNowFovY = 45.f;
			}
			m_pFPS_Camera->Set_FovY(fNowFovY);
		}
		else
		{
			if (!m_bBeholderAttack)
			{
				m_pBeholder->All_Round_Laser();
				m_pBeholder->All_Round_Laser_LandMine();

				m_bBeholderAttack = true;
			}
			else
			{
				m_fTimeAcc += fTimeDelta;
				if (m_fTimeAcc >= m_fEventDelay)
				{
					m_fTimeAcc = 0.f;
					m_bAnotherBranch = true;

					CPlayer_Manager::Get_Instance()->Camera_Shake_Order(0.2f, 100000.f);
					m_pPlayer->Set_CutScene(false);
					m_pPlayer->Change_SuperInvincible();

					m_pGameInstance->Play(L"Beholder_Instrument", true);
					m_pGameInstance->SetVolume(L"Beholder_Instrument", 1.f);

					m_pGameInstance->Stop(L"Boss_Level_Start_BGM");

					m_pBeholder->Set_CutScene(false);
					m_pFPS_Camera->Set_CutScene(false);
				}
			}
		}
	}
	else
	{
		if (fNowFovY < 90.f)
		{
			fNowFovY += 90.f * fTimeDelta;
			if (fNowFovY >= 90.f)
			{
				fNowFovY = 90.f;
			
				m_eState = Battle;
			}
				
			m_pFPS_Camera->Set_FovY(fNowFovY);
		}
	}

	
}


void CLevel_Boss::Free()
{
	__super::Free();

}
