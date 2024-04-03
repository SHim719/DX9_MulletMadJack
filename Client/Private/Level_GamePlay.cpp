
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Wall.h"
#include "Core_Camera.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device } 
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Wall_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Wall/Albedo/Wall%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Wall", TEXT("Prototype_Wall")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CCoreCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 0.f, -25.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.0f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Camera"), &CameraDesc) == nullptr)
		return E_FAIL;
	
	return S_OK;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}
