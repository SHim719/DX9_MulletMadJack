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
#include "Apollo.h"


CLevel_Boss::CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Boss::Initialize()
{
	m_iLevelID = LEVEL_BOSS;

	m_pGameInstance->Stop(L"Loading");

	CGame_Manager::Get_Instance()->Set_StageProgress(StageProgress::OnGoing);

	m_pGameInstance->Play(L"Beholder_Instrument", true);
	m_pGameInstance->SetVolume(L"Beholder_Instrument", 0.5f);

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Beholder(L"Monster")))
		return E_FAIL;

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/TestMh.dat", (LEVEL)m_iLevelID);
	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
	CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer_Manager::Get_Instance()->Get_NextWeapon());

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

	//jeongtest
	m_pGameInstance->Set_Ui_ActiveState(TEXT("CUi_BossHpBar"));

	return S_OK;
}

void CLevel_Boss::Tick(_float fTimeDelta)
{
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);
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
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(m_iLevelID, TEXT("Player"), TEXT("Prototype_Player")));
	CPlayer_Manager::Get_Instance()->Set_Player(pPlayer);

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	pPlayer->Set_PlayerHP(99.f);
	pPlayer->Get_Transform()->Set_Pos({ 0.f, 1.1f, 0.f });

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Beholder(const wstring& strLayerTag)
{
	CBeholder* TempBeholder = dynamic_cast<CBeholder*>
		(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag, TEXT("Prototype_Beholder")));
	CArtemis* TempArtemis = dynamic_cast<CArtemis*>(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
		TEXT("Prototype_Artemis")));
	CApollo* TempCApollo = dynamic_cast<CApollo*>(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
		TEXT("Prototype_Apollo")));
	if (TempBeholder == nullptr || TempArtemis == nullptr || TempCApollo == nullptr)
	{
		assert(false);
	}

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_BossHpBar"),
		eUiRenderType::Render_Blend,
		CUi_BossHpBar::Create(m_pGraphic_Device))))
		return E_FAIL;

	CUi_BossHpBar* pHpBar = dynamic_cast<CUi_BossHpBar*>
		(m_pGameInstance->Get_ActiveBlendUI(L"CUi_BossHpBar"));
	if (pHpBar == nullptr)
		assert(false);

	pHpBar->Set_Artemis(TempArtemis);
	pHpBar->Set_Beholder(TempBeholder);
	pHpBar->Set_Apollon(TempCApollo);

	///*if (nullptr == m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
	//	TEXT("Prototype_Artemis")))
	//	return E_FAIL;

	//if (nullptr == m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag,
	//	TEXT("Prototype_Apollo")))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Boss::Initialize_SodaMachine()
{
	CLayer* pMachineLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine");
	CLayer* pBannerLayer = m_pGameInstance->Find_Layer(m_iLevelID, L"SodaMachine_Banner");

	if (!pMachineLayer || !pBannerLayer)
		return;
	auto MachineLayerObjects = pMachineLayer->Get_GameObjects();
	auto BannerLayerObjects = pBannerLayer->Get_GameObjects();

	auto BannerIt = BannerLayerObjects.begin();
	for (auto it = MachineLayerObjects.begin(); it != MachineLayerObjects.end(); ++it)
	{
		dynamic_cast<CSodaMachine*>(*it)->Set_Banner(*BannerIt);

		++BannerIt;
	}
}


void CLevel_Boss::Free()
{

	__super::Free();

}
