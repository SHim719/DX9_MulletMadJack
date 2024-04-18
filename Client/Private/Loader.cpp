#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "White_Suit_Monster.h"
#include "Drone_Monster.h"
#include "Chainsaw_Monster.h"
#include "Orange_Pants_Monster.h"
#include "Enemy_Bullet.h"
#include "Player.h"
#include "Trigger_Headers.h"
#include "Ui_Include.h"
#include "Player_Include.h"
#include "CGame_Manager.h"
#include "MapObject_Header.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
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


	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	m_fProgress = 0.1f;
	lstrcpy(m_szLoadingText, TEXT("로딩중"));
	if (FAILED(Loading_For_WhiteSuitMonster()))
		return E_FAIL;

	if (FAILED(Loading_For_Map_Texture()))
		return E_FAIL;


	/*if (FAILED(Loading_For_Chainsaw_Monster()))
		return E_FAIL;

	if (FAILED(Loading_For_Orange_Pants_Monster()))
		return E_FAIL;

	if (FAILED(Loading_For_Drone_Monster()))
		return E_FAIL;*/

	m_fProgress = 0.6f;

	if (FAILED(Ready_MapObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Prototype()))
		return E_FAIL;
	
	/* For Prototype_GameObject_Drone_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Drone"),
	//	CDrone_Monster::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//
	///* For Prototype_GameObject_Chainsaw_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Chainsaw"),
	//	CChainsaw_Monster::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//
	///* For Prototype_GameObject_Orange_Pants_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Orange_Pants"),
	//	COrange_Pants_Monster::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//


	lstrcpy(m_szLoadingText, TEXT("UI."));
	if (FAILED(Loading_For_Ui()))
		return E_FAIL;

	Initialize_TextManager();

	lstrcpy(m_szLoadingText, TEXT("완."));
	m_fProgress = 1.f;

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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Enemy_Bullet_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Bullet0.png")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Map_Texture()
{
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Slope_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Slope/Slope0.png"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoader::Ready_MapObject_Prototype()
{
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Slope"),
		CSlope::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Chainsaw_Monster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/02bullethole/02bullethole%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Slash"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/attack/attack%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Bodyshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/bullethole/bullethole%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/defense/defense%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Pushed_Recovery"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/defense/defense%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_HeadExplode_Backward"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/flyback/flyback%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_end/Flyback_end%d.png"), 17))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Start_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_start/Flyback_start%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_wall/Flyback_wall%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_GetUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/GetUp/GetUp%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Groinshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Groinshot/Groinshot%d.png"), 26))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/hit/hit%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Idle_up/Idle_up%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_jump/ZVG_jump%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_Shotgun/ZVG_Shotgun%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_walk/ZVG_walk%d.png"), 12))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Monster_Prototype()
{
	/* For Prototype_GameObject_White_Suit_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_White_Suit"),
		CWhite_Suit_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Bullet"),
		CEnemy_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SpawnTrigger"),
		CSpawnTrigger::Create(m_pGraphic_Device))))
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
	return S_OK;
}


HRESULT CLoader::Loading_For_Orange_Pants_Monster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_2fly/vg_2fly%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_2nd_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_2hit/vg_2hit%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Aim"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_aim/vg_aim%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_blocking/vg_blocking%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Pushed_Recovery"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_blocking/vg_blocking%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Breathe"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_breathe/vg_breathe%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Bodyshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_death/vg_death%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_death_sg/vg_death_sg%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_flyback_floor_noshot/vg_flyback%d.png"), 31))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_flyback_wall_noshot/vg_flyback%d.png"), 27))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Groinshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_groinshot/vg_groinshot%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_HeadExplode"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_headexplode/VG_headexplode%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_hit/vg_hit%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_idle/vg_idle%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_jumping/vg_jumping%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Shot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_shot/vg_shot%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Orange_Pants_Monster_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Orange_Pants_Monster/vg_walk/vg_walk%d.png"), 14))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Drone_Monster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Aim"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_aim/air_aim%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_attack/air_attack%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_brain/Air_brain%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Reveal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/air_reveal/air_reveal%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Flyback"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/drone_flyback/drone_flyback%d.png"), 4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Ui()
{
	if (FAILED(Ready_ClearUi_Texture()))
		return E_FAIL;

	if (FAILED(Ready_ShopUi_Texture()))
		return E_FAIL;

	m_fProgress = 0.7f;

	if (FAILED(Ready_StartUi_Texture()))
		return E_FAIL;

	if (FAILED(Ready_OnGoingUi_Texture()))
		return E_FAIL;

	if (FAILED(Ready_TextUi_Texture()))
		return E_FAIL;

	if (FAILED(Ready_PlayerUi_Texture()))
		return E_FAIL;

	m_fProgress = 0.8f;

	if (FAILED(Ready_PlayerGunUi_Texture()))
		return E_FAIL;

	m_fProgress = 0.9f;

	if (FAILED(Ready_Camera_Effect_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Ui_Life()))
		return E_FAIL;

	if (FAILED(Ready_Active_UiClear()))
		return E_FAIL;



	if (FAILED(Ready_Active_UiShop()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Active_UiOnGoing()))
		return E_FAIL;

	if (FAILED(Ready_Active_Gun()))
		return E_FAIL;

	if (FAILED(Ready_Active_Camera_Effect()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_ClearUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Peace_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Green_Peace.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Fine_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Fine.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_LiveStream_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/LiveStream.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_BackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Beat_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_Beat.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Heart_Line_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Heart/Heart_Line.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Border_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Border.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_BorderRight_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/BorderRight.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Chat_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Chat/Chat%d.png", 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Announcer_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Announcer/Announcer.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Sheet_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Sheet/Sheet%d.png", 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Clear_Right_Middle.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Word_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Clear_Floor.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_Logo_Num_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Clear_Font/Clear_%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Clear_Time_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Victory/FloorTime.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Real_ClearTime_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Victory/Green_%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_ClearTime_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Time_BackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Time_Division_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Logo/Time_Division.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Clear_Victory_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Victory/Victory.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_ShopUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Shop_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/Clear_Shop%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Shop_UpGrade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_UpGrade_Select_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade_Select%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_UpGrade_Focus_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade_Focus.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_StartUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_F_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_F%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_L_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_L%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_O_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_O%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_R_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start_R%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Floor_0_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Start/Start%d.png", 6))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_OnGoingUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Background_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Ui/Life/Ui_Life_Background.png"))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Finish_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Dash_Finish_Stroke.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Finish_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Finish_BackGround.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_DrinkSoda_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Drink_Soda.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Damaged_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Damaged.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Combo_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Combo%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_SpecialHit_Part_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Part/PartSec%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Reload_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Reload.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_TextUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CText_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Text/TextBackGround.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_PlayerUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CrossHair_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Crosshair/Crosshair%d.png", 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Hand_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Hand/Pistol/HAND_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Idle/PISTOL_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Kick_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Kick/Kick%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Phone_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/LeftArm/LeftArm%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Phone_Number_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/LeftArm/Phone_Font_%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Phone_Face_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/LeftArm/PhoneFace%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Phone_BackGround_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/LeftArm/Phone_BackGround%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Drink_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Drink/DRINK%d.png", 7))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_PlayerGunUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Idle_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Idle/PISTOL_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Shot_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Shot/PISTOL_SHOT%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Spin_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Spin/SPIN_AIR%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Reload_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload/PISTOL_RELOAD%d.png", 16))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Camera_Dash_Textures",
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
	//		L"../Bin/Resources/Textures/Camera/Dash/CircleLines%d.png", 6))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Fire_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Gunfire/PISTOL_MUZZLE%d.png", 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Barrel_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload_Barrel/BARREL%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Reload_Arm_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload_Arm/Reload_Arm%d.png", 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Pistol_Reload_Hand_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Pistol/Reload_Hand/HAND_PISTOL_RELOAD%d.png", 5))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Camera_Effect_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Camera_Dash_Textures", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Camera/Dash/CircleLines%d.png", 6))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Prototype_Ui_Life()
{
	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_MonsterDie"),
		CUi_MonsterDie::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_BackGround"),
		CUi_Background::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Special3Sec"),
		CUi_Special3Sec::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_SpecialHit"),
		CUi_SpecialHit::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_SpecialHit_Part"),
		CUi_SpecialHit_Part::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Floor_Part"),
		CUi_Floor_Part::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Shop_UpGrade"),
		CUi_Shop_UpGrade::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_UpGrade_Select"),
		CUi_UpGrade_Select::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_UpGrade_Focus"),
		CUi_UpGrade_Focus::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Floor_Logo_Num"),
		CUi_Floor_Logo_Num::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Real_ClearTime"),
		CUi_Real_ClearTime::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_ClearTime_BackGround"),
		CUi_ClearTime_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Time_Division"),
		CUi_Time_Division::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CText_BackGround"),
		CText_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Combo"),
		CUi_Combo::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_UiClear()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Peace"),
		eUiRenderType::Render_NonBlend,
		CUi_PEACE::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Fine"),
		eUiRenderType::Render_NonBlend,
		CUi_Fine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart_BackGround"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Heart_Line"),
		eUiRenderType::Render_NonBlend,
		CUi_Heart_Line::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Border"),
		eUiRenderType::Render_NonBlend,
		CUi_Border::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Chat"),
		eUiRenderType::Render_NonBlend,
		CUi_Chat::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_LiveStream"),
		eUiRenderType::Render_NonBlend,
		CUi_LiveStream::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Announcer"),
		eUiRenderType::Render_NonBlend,
		CUi_Announcer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor_Logo"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor_Logo_Word"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor_Logo_Word::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Floor"),
		eUiRenderType::Render_NonBlend,
		CUi_Floor::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Clear_Time"),
		eUiRenderType::Render_NonBlend,
		CUi_Clear_Time::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Clear_Victory"),
		eUiRenderType::Render_NonBlend,
		CUi_Clear_Victory::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_UiShop()
{
	if (FAILED(m_pGameInstance->Add_Ui_ShopActive(TEXT("CUi_Shop"),
		CUi_Shop::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_UiOnGoing()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Finish"),
		eUiRenderType::Render_NonBlend,
		CUi_Finish::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_DrinkSoda"),
		eUiRenderType::Render_Blend,
		CUi_DrinkSoda::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Damaged"),
		eUiRenderType::Render_Blend,
		CUi_Damaged::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Reload"),
		eUiRenderType::Render_NonBlend,
		CUi_Reload::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_Gun()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_CrossHair", eUiRenderType::Render_NonBlend, CCrossHair::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Right_Hand", eUiRenderType::Render_NonBlend, CPistol_Right_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol", eUiRenderType::Render_NonBlend, CPistol::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Shot", eUiRenderType::Render_Blend, CPistol_Shot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Spin", eUiRenderType::Render_NonBlend, CPistol_Spin::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload", eUiRenderType::Render_NonBlend, CPistol_Reload::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Barrel", eUiRenderType::Render_NonBlend, CPistol_Barrel::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Kick", eUiRenderType::Render_NonBlend, CKick::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload_Arm", eUiRenderType::Render_NonBlend, CPistol_Reload_Left_Arm::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload_Hand", eUiRenderType::Render_NonBlend, CPistol_Reload_Left_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Phone", eUiRenderType::Render_NonBlend, CPhone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Drink", eUiRenderType::Render_NonBlend, CDrink::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Prototype_Effect()
{
	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CPistol_Gunfire"),
		CPistol_Gunfire::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_Camera_Effect()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Camera_Dash", eUiRenderType::Render_Blend, CDash_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

void CLoader::Initialize_TextManager()
{
	CGame_Manager::Get_Instance()->Initialize_TextManager();
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
