#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "White_Suit_Monster.h"
#include "Drone_Monster.h"
#include "Chainsaw_Monster.h"
#include "Enemy_Bullet.h"
#include "Wall.h"
#include "Floor.h"
#include "MapObject.h"
#include "SodaMachine.h"
#include "SodaMachine_Banner.h"
#include "Border.h"
#include "Soda.h"
#include "Door.h"
#include "Player.h"

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

	if (FAILED(Loading_For_WhiteSuitMonster()))
		return E_FAIL;

#pragma region TEXTURE_DRONE_MONSTER
//	///* For Prototype_Component_Texture_Drone_Monster */
//	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Aim"),
//	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_aim/air_aim%d.png"), 6))))
//	//	return E_FAIL;
//
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Attack"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_attack/air_attack%d.png"), 10))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Idle"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_brain/Air_brain%d.png"), 6))))
//		return E_FAIL;
	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_BodyShot"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/fb_death/fb_death%d.png"), 22))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Headshot"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/flyb_death/flyb_death%d.png"), 21))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Shot_Floor"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/flyback_endo/flyback_endo%d.png"), 31))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Shot_Wall"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/flyback_wall_endo/flyback_wall_endo%d.png"), 28))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_GroinShot"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/groinshot/BG_groinshot%d.png"), 23))))
//		return E_FAIL;
////	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_HeadExplode"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/headexplode/headexplode%d.png"), 22))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_1st_Hit"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/hit/hit%d.png"), 8))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Idle"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/idle/idle%d.png"), 9))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Backwards_Machinegun"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/sd_backwards/sd_backwards%d.png"), 27))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Shoot"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/shoot/shoot%d.png"), 8))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Walk"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/walk/walk%d.png"), 13))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Push_Floor"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback/ws_flyback%d.png"), 31))))
//		return E_FAIL;
//	
//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Push_Wall"),
//		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback_wall/ws_flyback_wall%d.png"), 28))))
//		return E_FAIL;
//
#pragma endregion
#pragma region TEXTURE_CHAINSAW_MONSTER
	/* For Prototype_Component_Texture_Drone_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Headshot"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/02bullethole/02bullethole%d.png"), 23))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Slash"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/attack/attack%d.png"), 9))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_BodyShot"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/bullethole/bullethole%d.png"), 22))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Blocking"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/defense/defense%d.png"), 6))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_HeadExplode_Backward"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/flyback/flyback%d.png"), 21))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Floor"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_end/Flyback_end%d.png"), 17))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Start_Push_Floor"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_start/Flyback_start%d.png"), 14))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Wall"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_wall/Flyback_wall%d.png"), 14))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_GetUp"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/GetUp/GetUp%d.png"), 12))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_GroinShot"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Groinshot/Groinshot%d.png"), 26))))
	//	return E_FAIL;
	//
	////if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_HeadExplode_Forward"),
	////	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/headless/headless%d.png"), 23))))
	////	return E_FAIL;
	//
	////if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Hit"),
	////	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/hit/hit%d.png"), 4))))
	////	return E_FAIL;
	//
	////if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Idle_Down"),
	////	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Idle_down/Idle_down%d.png"), 10))))
	////	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Idle_Up"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Idle_up/Idle_up%d.png"), 7))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Shot_Floor"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_fly/ZVG_fly%d.png"), 30))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Jump"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_jump/ZVG_jump%d.png"), 22))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Shotgun"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_Shotgun/ZVG_Shotgun%d.png"), 23))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Walk"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_walk/ZVG_walk%d.png"), 12))))
	//	return E_FAIL;
   //
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Wall"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_wall/ZVG_wall%d.png"), 13))))
	//	return E_FAIL;

#pragma endregion
#pragma region ObjectTextures
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Wall_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Wall/Albedo/Wall%d.png", 17))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Floor_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Floor/Albedo/Floor%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Border_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Border/Border%d.png", 1))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Object_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Objects/Object%d.png", 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Soda_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/SodaMachine/SodaMachine%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"SodaCan_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Soda/Soda0.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Door_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Door/Door%d.png", 5))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));

#pragma region MapObjectsPrototype
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Floor"),
		CFloor::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_MapObject"),
		CMapObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SodaMachine"),
		CSodaMachine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SodaMachine_Banner"),
		CSodaMachine_Banner::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Border"),
		CBorder::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Soda"),
		CSoda::Create(m_pGraphic_Device))))
		return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Door"),
        CDoor::Create(m_pGraphic_Device))))
        return E_FAIL;

#pragma endregion

	/* For Prototype_GameObject_White_Suit_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_White_Suit"),
		CWhite_Suit_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;
	//
	///* For Prototype_GameObject_Drone_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Drone_Monster"),
	//	CDrone_Monster::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//
	///* For Prototype_GameObject_Chainsaw_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chainsaw_Monster"),
	//	CChainsaw_Monster::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//
	///* For Prototype_GameObject_Enemy_Bullet */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Bullet"),
	//	CEnemy_Bullet::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	
	lstrcpy(m_szLoadingText, TEXT("UI을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_WhiteSuitMonster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/idle/idle%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Shot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/shoot/shoot%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/walk/walk%d.png"), 13))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/blocking/bg_blocking%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Pushed_Recovery"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/blocking/bg_blocking%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/jump/jump%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Aim"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/aim/aim%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/death_shotgun/death_shotgun%d.png"), 23))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Bodyshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/fb_death/fb_death%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/flyb_death/flyb_death%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Groinshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/groinshot/BG_groinshot%d.png"), 23))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_HeadExplode"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/headexplode/headexplode%d.png"), 22))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/hit/hit%d.png"), 8))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback/ws_flyback%d.png"), 31))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback_wall/ws_flyback_wall%d.png"), 28))))
		return E_FAIL;

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
