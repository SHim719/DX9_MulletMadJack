#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "CUi_MonsterDie.h"
#include "CUi_Special3Sec.h"
#include "CUi_SpecialHit.h"


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device } 
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_iLevelID = LEVEL_LOADING;  

	m_eNextLevelID = eNextLevelID;

	/* 로딩 레벨에 보여주기위한 객체(배경, 로딩바, 로딩상태폰트)들을 생성한다. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* 자원로딩을 위한 로더객체를 생성해준다. */
	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;	

	if (FAILED(Test_Texture_Loading()))
		return E_FAIL;

	//if (FAILED(Test_LifeUi_Clone()))
		//return E_FAIL;


	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{

	if (true == m_pLoader->isFinished())
	{
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
	
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Loading::Test_LifeUi_Clone()
{
	//CUi_MonsterDie::MonsterDie_Arg Arg;
	//Arg.MonsterDiePosX = -200;
	//Arg.MonsterDiePosY = -200;
	//Arg.MonsterGrade = eMonsterGrade::Special;
	//if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_MonsterDie"), 
	//	eUiRenderType::Render_Blend,
	//	&Arg)))
	//	return E_FAIL;
	//
	/*if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_Special3Sec"), 
		eUiRenderType::Render_Blend, 
		nullptr
		)))
		return E_FAIL;*/
	
	//_uint a = (_uint)eSpecialHit::HEADSHOT;
	//if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &a)))
		//return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Test_Texture_Loading()
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

	Safe_Release(m_pLoader);
}
