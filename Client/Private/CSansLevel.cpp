#include "CSansLevel.h"
#include "MapLoader.h"
#include "PlayerManager.h"
#include "CSans.h"


CSansLevel::CSansLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CSansLevel::Initialize()
{
	m_iLevelID = LEVEL_SANS;
	m_pGameInstance->Play(L"Gameplay", true);
	m_pGameInstance->SetVolume(L"Gameplay", 0.5f);

	CGame_Manager::Get_Instance()->Set_StageProgress(StageProgress::OnGoing);

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sans_Boss(L"Layer_Sans")))
		return E_FAIL;

	CMapLoader::Get_Instance()->Load(L"../Bin/Resources/DataFiles/Sans.dat", (LEVEL)m_iLevelID);
	CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
	m_pPlayer->SansLevelEnterInitialize();

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

	return S_OK;
}

void CSansLevel::Tick(_float fTimeDelta)
{
	m_itesttime += fTimeDelta;
	CPlayer_Manager::Get_Instance()->Tick_AdjustTime(fTimeDelta, 8.f);
	if (m_pSans->GetSansTurnInfo() == SansTurnBased::SansText)
	{
		if (m_itesttime > 4)
		{
			m_itesttime = 0;
			cout << "sansText" << endl;
		}
		if(m_pGameInstance->GetKeyDown(eKeyCode::RButton))
		{
			m_pSans->SetTurn(SansTurnBased::SansTurn);
			CGame_Manager::Get_Instance()->Clear_Sans_Text();
			m_pGameInstance->Set_Ui_ActiveState
			(L"CUi_Sans_TextBack", false);	
		}
	}
	else if (m_pSans->GetSansTurnInfo() == SansTurnBased::SansTurn)
	{
		if (m_itesttime > 4)
		{
			m_itesttime = 0;
			cout << "sansTurn" << endl;
		}
		// 움직임만 가능
	}
	else if (m_pSans->GetSansTurnInfo() == SansTurnBased::PlayerTurn)
	{
		if (m_itesttime > 4)
		{
			m_itesttime = 0;
			cout << "PlayerTurn" << endl;
		}
		if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
		{
			m_pSans->SetTurn(SansTurnBased::SansText);
		}
	}

	if (m_pPlayer->Get_PlayerHP() <= 0)
	{
		m_pPlayer->SansLevelExitInitialize();
		// jaewook SceneChange 
	}
}

HRESULT CSansLevel::Render()
{
	return S_OK;
}

HRESULT CSansLevel::Ready_Layer_Camera(const wstring& strLayerTag)
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

HRESULT CSansLevel::Ready_Layer_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Player"), TEXT("Prototype_Player")));
	CPlayer_Manager::Get_Instance()->Set_Player(pPlayer);
	
	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	m_pPlayer = pPlayer;
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

HRESULT CSansLevel::Ready_Layer_Sans_Boss(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_SANS, strLayerTag,
		TEXT("Prototype_Sans")))
		return E_FAIL;

	m_pSans = (CSans*)m_pGameInstance->Find_GameObject(LEVEL_SANS, L"Layer_Sans", 0);
	Safe_AddRef(m_pSans);

	return S_OK;
}

CSansLevel* CSansLevel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSansLevel* pInstance = new CSansLevel(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CSansLevel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSansLevel::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pFPS_Camera);
	Safe_Release(m_pSans);
	__super::Free();
}
