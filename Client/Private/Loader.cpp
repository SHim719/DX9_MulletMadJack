#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Machine_Gun.h"

#include "GameInstance.h"
#include "Enemy.h"
#include "Enemy_Bullet.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

// typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
unsigned int APIENTRY Loading_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;	

	return pLoader->Loading();	
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

unsigned int CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr{};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;
}

HRESULT CLoader::Loading_For_Logo_Level()
{	
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	
#pragma region TEXTURE_ENEMY
	/* For Prototype_Component_Texture_Enemy */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_2nd_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/2hit/2hit%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Aim"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/aim/aim%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Blocking"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/blocking/bg_blocking%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Breathe"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/breathe/breathe%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/death_shotgun/death_shotgun%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Machinegun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/fb_death/fb_death%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/flyb_death/flyb_death%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shot_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/flyback_endo/flyback_endo%d.png"), 31))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shot_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/flyback_wall_endo/flyback_wall_endo%d.png"), 28))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_GroinShot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/groinshot/BG_groinshot%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_HeadExplode"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/headexplode/headexplode%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_1st_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/hit/hit%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/idle/idle%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Backwards_Machinegun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/sd_backwards/sd_backwards%d.png"), 27))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Shoot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/shoot/shoot%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/walk/walk%d.png"), 13))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/ws_flyback/ws_flyback%d.png"), 31))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Enemy/ws_flyback_wall/ws_flyback_wall%d.png"), 28))))
		return E_FAIL;

#pragma endregion



	/* For Prototype_Component_Texture_Enemy_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/bullet_sprites_4_%d.png"), 4))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));

	/* For Prototype_GameObject_Enemy */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy"),
		CEnemy::Create(m_pGraphic_Device))))
		return E_FAIL;



	/* For Prototype_GameObject_Enemy_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Bullet"),
		CEnemy_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;


	
	lstrcpy(m_szLoadingText, TEXT("UI을(를) 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_CrossHair", eUiRenderType::Render_NonBlend, CMachine_Gun::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
