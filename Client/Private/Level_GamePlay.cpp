#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Machine_Gun.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device } 
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;

	//Load_MapObject(L"../Bin/Resources/DataFiles/TestMap.dat", OBJTYPE_END);

	auto wall = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Wall", TEXT("Prototype_Wall"));
	wall->Get_Transform()->Set_Position(_float3(0.f, 0.f, 0.f));
	wall->Set_Texture_Index(1);


	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			wall = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Wall", TEXT("Prototype_Wall"));
			wall->Get_Transform()->Rotation_XYZ(_float3(90.f, 0.f, 0.f));
			wall->Get_Transform()->Set_Position(_float3((float)i, -0.5f, (float)j));

			if (i == 9) {

			wall = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Wall", TEXT("Prototype_Wall"));
			wall->Get_Transform()->Rotation_XYZ(_float3(0.f, 90.f, 0.f));
			wall->Get_Transform()->Set_Position(_float3((float)i, 0.f, (float)j));

			}

			if(j == 9) {
				wall = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Wall", TEXT("Prototype_Wall"));
				wall->Get_Transform()->Rotation_XYZ(_float3(0.f, 0.f, 0.f));
				wall->Get_Transform()->Set_Position(_float3((float)i, 0.f, (float)j));
			}
		}
	}

	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"),true);

	//if (FAILED(Test_UiTexture_Loading()))
	//	return E_FAIL;

	//if (FAILED(Test_LifeUi_Clone()))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Enemy(TEXT("Layer_Enemy"))))
	//	return E_FAIL;

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

	CameraDesc.vEye = _float3(0.f, 0.2f, -1.f);
	CameraDesc.vAt = _float3(10.f, 0.f, 10.f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.0f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	// FPS 카메라 사본을 pFPS_Camera에 담음
	if(FAILED(m_pGameInstance->Create_Camera(strLayerTag, CFPS_Camera::Create(m_pGraphic_Device, &CameraDesc))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Enemy(const wstring& strLayerTag)
{
	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Enemy")))
		return E_FAIL;

	if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Enemy_Bullet")))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}

HRESULT CLevel_GamePlay::Test_LifeUi_Clone()
{
	//CUi_MonsterDie::MonsterDie_Arg Arg;
	//Arg.MonsterDiePosX = -200;
	//Arg.MonsterDiePosY = -200;
	//Arg.MonsterGrade = eMonsterGrade::High;
	//if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_MonsterDie"),
	//	eUiRenderType::Render_Blend,
	//	&Arg)))
	//	return E_FAIL;


	//if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_Special3Sec"),
	//	eUiRenderType::Render_Blend,
	//	nullptr)))
	//	return E_FAIL;


	//CUi_SpecialHit::SpecialHit_Desc Arg;

	//Arg.Hit = eSpecialHit::HEADSHOT;
	//Arg.iCount = 4;
	//if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"),
	//	eUiRenderType::Render_NonBlend,
	//	&Arg)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Test_UiTexture_Loading()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Background_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Ui_Life_Background.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterLowGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/1Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterMiddleGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/2Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_MonsterHighGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/3Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Special3Sec_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/Special3Sec.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_HEADSHOT_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/HEADSHOT.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_FINISHED_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/FINISHED.png"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Peace_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/GREEN_PEACE.png"))))
		return E_FAIL;


	return S_OK;
}
