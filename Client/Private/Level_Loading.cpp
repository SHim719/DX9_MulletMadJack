#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Elevator_Level.h"
#include "CUi_LoadingBackGround.h"
#include "Level_Map2.h"
#include "CSansLevel.h"


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device } 
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_iLevelID = LEVEL_LOADING;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Menu.wav", L"Loading")))
		return E_FAIL;

	m_pGameInstance->Play(L"Loading", true);
	m_pGameInstance->SetVolume(L"Loading", 0.5f);

	m_eNextLevelID = eNextLevelID;

	m_pLoadingBackGround = (CUi_LoadingBackGround*)m_pGameInstance->Add_Ui_PartClone
	(L"CUi_LoadingBackGround", nullptr);
	/* �ε� ������ �����ֱ����� ��ü(���, �ε���, �ε�������Ʈ)���� �����Ѵ�. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* �ڿ��ε��� ���� �δ���ü�� �������ش�. */
	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;	

	//if (FAILED(Test_Texture_Loading()))
	//	return E_FAIL;

	//if (FAILED(Test_LifeUi_Clone()))
		//return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	m_pLoadingBackGround->Tick(fTimeDelta);
	m_pLoadingBackGround->Set_BarRatio(m_pLoader->Get_ProgressPercent());
	if (true == m_pLoader->isFinished())
	{
		int a = 10;
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			CLevel*		pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			case LEVEL_ELEVATOR:
				pLevel = CElevator_Level::Create(m_pGraphic_Device);
			case LEVEL_SANS:
				pLevel = CSansLevel::Create(m_pGraphic_Device);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(pLevel)))
				return;
		}		
	}
}

HRESULT CLevel_Loading::Render()
{	
	m_pLoader->Show_LoadingText();

	m_pLoadingBackGround->Render();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoadingBackGround);
	Safe_Release(m_pLoader);
}
