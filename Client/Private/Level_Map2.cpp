#include "Level_Map2.h"
#include "GameInstance.h"
#include "CGame_Manager.h"

#include "FPS_Camera.h"
#include "PlayerManager.h"
#include "Player.h"

#include "MapLoader.h"
#include "MapObject_Header.h"
#include "Trigger_Headers.h"



CLevel_Map2::CLevel_Map2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Map2::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY2;
	m_pGameInstance->Play(L"Gameplay2", true);
	m_pGameInstance->SetVolume(L"Gameplay2", 0.5f);

	//if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
	//	return E_FAIL;
	//
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	//CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Set_Pos({ 0.f, 1.1f, 0.f });

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Test3.dat", (LEVEL)m_iLevelID);

	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);

	Initialize_SodaMachine();

	static_cast<CStageEndTrigger*>(m_pGameInstance->Find_GameObject(m_iLevelID, L"Trigger", 0))->Set_NextLevel
	(LEVEL_ELEVATOR);

	return S_OK;
}

void CLevel_Map2::Tick(_float fTimeDelta)
{
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);
}

HRESULT CLevel_Map2::Render()
{
	return S_OK;
}


CLevel_Map2* CLevel_Map2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Map2* pInstance = new CLevel_Map2(pGraphic_Device);

	//if (FAILED(pInstance->Initialize()))
	//{
	//	MSG_BOX(TEXT("Failed to Created : CLevel_Map2"));
	//	Safe_Release(pInstance);
	//}

	return pInstance;
}

HRESULT CLevel_Map2::Ready_Layer_Camera(const wstring& strLayerTag)
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

HRESULT CLevel_Map2::Ready_Layer_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(m_iLevelID, TEXT("Player"), TEXT("Prototype_Player")));
	CPlayer_Manager::Get_Instance()->Set_Player(pPlayer);

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	pPlayer->Get_Transform()->Set_Pos({ 0.f, 1.1f, 0.f });

	return S_OK;
}

void CLevel_Map2::Initialize_SodaMachine()
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


void CLevel_Map2::Free()
{

	__super::Free();

}
