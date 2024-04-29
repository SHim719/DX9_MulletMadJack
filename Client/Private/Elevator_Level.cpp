#include "Elevator_Level.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "CGame_Manager.h"
#include "Enemy_Bullet.h"

#include "Core_Camera.h"
#include "PlayerManager.h"
#include "Player.h"

#include "MapLoader.h"

#include "ElevatorLevelManager.h"



CElevator_Level::CElevator_Level(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CElevator_Level::Initialize()
{
	m_iLevelID = LEVEL_ELEVATOR;
	m_pGameInstance->Stop(L"Loading");
	m_pGameInstance->Stop(L"Elevator_FX");

	Ready_Layer_Camera(TEXT("Main_Camera"));

	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/ElevatorMap.dat", (LEVEL)m_iLevelID);

	m_pGameInstance->Get_CurCamera()->Get_Transform()->LookAt(_float3(0.5f, 0.5f, 10.f));

	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
	CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer_Manager::Get_Instance()->Get_NextWeapon());

	D3DLIGHT9 lightDesc{};
	lightDesc.Type = D3DLIGHT_DIRECTIONAL;
	lightDesc.Diffuse = D3DXCOLOR(1.f, 0.6f, 0.8f, 1.f);
	lightDesc.Ambient = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
	lightDesc.Direction = _float3(0.f, -1.f, 0.f);
	
	if (FAILED(m_pGraphic_Device->SetLight(9, &lightDesc)))
		return E_FAIL;
	
	D3DMATERIAL9		MaterialDesc{};
	MaterialDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MaterialDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);

	m_pGraphic_Device->SetMaterial(&MaterialDesc);

	m_pGraphic_Device->LightEnable(9, TRUE);

	CElevatorLevelManager::Get_Instance()->Initialize(m_pGraphic_Device);

	m_pGameInstance->Play(L"Elevator_Level_SFX", true);
	m_pGameInstance->SetVolume(L"Elevator_Level_SFX", 0.3f);

	return S_OK;
}

void CElevator_Level::Tick(_float fTimeDelta)
{
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);
	CElevatorLevelManager::Get_Instance()->Tick(fTimeDelta);
}

HRESULT CElevator_Level::Render()
{
	return S_OK;
}


CElevator_Level* CElevator_Level::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CElevator_Level* pInstance = new CElevator_Level(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CElevator_Level"));
		Safe_Release(pInstance);
	}

	return pInstance;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}

HRESULT CElevator_Level::Ready_Layer_Camera(const wstring& strLayerTag)
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

HRESULT CElevator_Level::Ready_Layer_Player()
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


void CElevator_Level::Free()
{

	__super::Free();


}
