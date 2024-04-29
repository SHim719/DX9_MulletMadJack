#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"
#include "Player.h"
#include "Trigger_Headers.h"
#include "Monster_Headers.h"
#include "Ui_Include.h"
#include "Player_Include.h"
#include "CGame_Manager.h"
#include "MapObject_Header.h"
#include "Effect_Headers.h"
#include "Sans_Include.h"
#include "CSkyBox.h"


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
	case LEVEL_LOBBY:
		hr = Loading_For_Lobby_Level();
		break;
	case LEVEL_STATIC:
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL_GAMEPLAY:
	case LEVEL_ELEVATOR:
	case LEVEL_SANS:
	case LEVEL_BOSS:
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
	lstrcpy(m_szLoadingText, TEXT("LOADING.."));
	if (FAILED(Loading_For_WhiteSuitMonster()))
		return E_FAIL;

	if (FAILED(Loading_For_Chainsaw_Monster()))
		return E_FAIL;

	if (FAILED(Loading_For_Drone_Monster()))
		return E_FAIL;

	m_fProgress = 0.3f;

	if (FAILED(Loading_For_Map_Texture()))
		return E_FAIL;

	if (FAILED(Loading_For_Effect_Texture()))
		return E_FAIL;

	if (FAILED(Loading_For_Sans_Texture()))
		return E_FAIL;

	if (FAILED(Loading_For_Boss_Texture()))
		return E_FAIL;

	m_fProgress = 0.6f;

	if (FAILED(Ready_MapObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Effect_Prototype()))
		return E_FAIL;

	if (FAILED(Loading_For_Ui()))
		return E_FAIL;

	if (FAILED(Ready_BGM()))
		return E_FAIL;

	if (FAILED(Ready_Player_Weapon_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Player_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Effect_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Announcer_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Streamer_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Beholder_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Sans_Sound()))
		return E_FAIL;

	if (FAILED(Ready_SkyBox()))
		return E_FAIL;

	Initialize_TextManager();

	lstrcpy(m_szLoadingText, TEXT("Wan."));
	m_fProgress = 1.f;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss_Level()
{
	if (FAILED(Loading_For_Map_Texture()))
		return E_FAIL;

	if (FAILED(Loading_For_WhiteSuitMonster()))
		return E_FAIL;

	if (FAILED(Loading_For_Chainsaw_Monster()))
		return E_FAIL;

	if (FAILED(Loading_For_Drone_Monster()))
		return E_FAIL;

	if (FAILED(Loading_For_Boss_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_MapObject_Prototype()))
		return E_FAIL;

	if (FAILED(Loading_For_Ui()))
		return E_FAIL;

	if (FAILED(Loading_For_Effect_Texture()))
		return E_FAIL;

	if(FAILED(Ready_Effect_Prototype()))
		return E_FAIL;


#pragma region Sound
	if (FAILED(Ready_BGM()))
		return E_FAIL;

	if (FAILED(Ready_Player_Weapon_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Player_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Effect_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Announcer_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Streamer_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Beholder_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Sans_Sound()))
		return E_FAIL;
#pragma endregion

	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Texture_TrackingLaser"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Sans/GasterLaser.png")))))
		return E_FAIL;*/

	if (FAILED(Ready_SkyBox()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("End."));
	m_fProgress = 1.f;

	m_isFinished = true;

	return S_OK;

}

HRESULT CLoader::Loading_For_Lobby_Level()
{
	lstrcpy(m_szLoadingText, TEXT("End."));

	m_fProgress = 1.f;
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Sans_Level()
{
	if (FAILED(Loading_For_Map_Texture()))
		return E_FAIL;

	if (FAILED(Loading_For_Sans_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Effect_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_MapObject_Prototype()))
		return E_FAIL;

	if (FAILED(Loading_For_Ui()))
		return E_FAIL;

	if (FAILED(Ready_BGM()))
		return E_FAIL;

	if (FAILED(Ready_Player_Weapon_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Player_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Monster_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Effect_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Beholder_Sound()))
		return E_FAIL;

	if (FAILED(Ready_Sans_Sound()))
		return E_FAIL;

	Initialize_TextManager();

	lstrcpy(m_szLoadingText, TEXT("Wan."));
	m_fProgress = 1.f;

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_WhiteSuitMonster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/idle/idle%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Shot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/shoot/shoot%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/walk/walk%d.png"), 13))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/blocking/bg_blocking%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Pushed_Recovery"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/blocking/bg_blocking%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/jump/jump%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Aim"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/aim/aim%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/death_shotgun/death_shotgun%d.png"), 23))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Bodyshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/fb_death/fb_death%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/flyb_death/flyb_death%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Groinshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/groinshot/BG_groinshot%d.png"), 23))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_HeadExplode"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/headexplode/headexplode%d.png"), 22))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/hit/hit%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Fly"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/fly/ws_flyback_wall%d.png"), 13))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback/ws_flyback%d.png"), 17))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/ws_flyback_wall/ws_flyback_wall%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Katana_Left_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/Katana_Left_Corpse/ws_Left_CorpseUp%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Katana_Left_Bottom"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/Katana_Left_Bottom_Corpse/ws_Left_Corpse_Down%d.png"), 20))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Air"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/Air/Air%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_White_Suit_Landing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/White_Suit_Monster/Landing/Landing%d.png"), 11))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Enemy_Bullet_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Bullet0.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Enemy_Missile_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Missile0.png")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Map_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Wall_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Wall/Albedo/Wall%d.png", 25))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Floor_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Floor/Albedo/Floor%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Border_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Border/Border%d.png", 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Object_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Objects/Object%d.png", 31))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"BoxObject_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Box/Box0.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Elevator_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Elevator/Elevator0.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Ventilador_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ventilador/Ventilador%d.png", 2))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Effect_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Explosion_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Effect/Explosion/Boom%d.png", 20))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"HitBlood_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Effect/HitBlood/Blood%d.png", 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"BloodKatanaEffect_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Effect/HitKatana/BloodKatana%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Starlight_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Effect/Starlight/%d.png", 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Blink_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Effect/Blink/%d.png", 9))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Sans_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Sans_Body"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Sans_Boss/SansBody.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Sans_Leg"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Sans_Boss/Sans_Leg.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Sans_Bone"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Sans/HalfBone.png")))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_GasterLaser"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Sans/GasterLaser.png")))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CSans_Gaster_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, 
			TEXT("../Bin/Resources/Textures/Pawn/Sans_Boss/Gaster/GasterBlaster%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CUi_Sans_Heart_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Ui/Part/SansHeart.png")))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CUi_Sans_Black_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Pawn/Sans_Boss/CUi_Sans_Black.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CSansMiss_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Pawn/Sans_Boss/CSansMiss.png")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Beholder_Idle_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Idle/BEACON%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Beholder_Damaged_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Damaged/BEACON%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Beholder_Groggy_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Groggy/BEACON%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Beholder_Idle2_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Charge/BEACON%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Beholder_Wait_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Close/BEACON%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"LandMine_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/LandMine/LandMine.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Artemis_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Artemis/Idle/Artemis_%d.png"),5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Artemis_Attack_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Artemis/Attack/Artemis_%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Apollo_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Apollo/Idle/Apollo_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Apollo_Attack_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			TEXT("../Bin/Resources/Textures/Boss/Apollo/Attack/Apollo_%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Texture_TrackingLaser"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Bullet/Sans/GasterLaser.png")))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_BoxObject"),
		CBoxObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Ventilador"),
		CVentilador::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Elevator_L"),
		CElevator_L::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Elevator_R"),
		CElevator_R::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_MoveWall"),
		CMoveWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_StageEndTrigger"),
		CStageEndTrigger::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_BossEntryTrigger"),
		CBossEntryTrigger::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_LandMine"),
		CLandMine::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Chainsaw_Monster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Headshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/02bullethole/02bullethole%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Slash"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/attack/attack%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Bodyshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/bullethole/bullethole%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/defense/defense%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Pushed_Recovery"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/defense/defense%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_HeadExplode_Backward"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/flyback/flyback%d.png"), 21))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_end/Flyback_end%d.png"), 17))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Start_Push_Floor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_start/Flyback_start%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Push_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Flyback_wall/Flyback_wall%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_GetUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/GetUp/GetUp%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Groinshot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Groinshot/Groinshot%d.png"), 26))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/hit/hit%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Idle_up/Idle_up%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_jump/ZVG_jump%d.png"), 22))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Shotgun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_Shotgun/ZVG_Shotgun%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Walk"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/ZVG_walk/ZVG_walk%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Katana_Left_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Katana_Left_Up/Saw_Katana_Up%d.png"), 28))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Katana_Left_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Katana_Left_Down/Saw_Katana_Left_Down%d.png"), 23))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Air"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Air/Air%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Chainsaw_Landing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Chainsaw_Monster/Landing/Landing%d.png"), 10))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Monster_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_White_Suit"),
		CWhite_Suit_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Chainsaw"),
		CChainsaw_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Drone"),
		CDrone_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_White_Suit_Slope"),
		CWhite_Suit_Slope::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Corpse"),
		CEnemy_Corpse::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Bullet"),
		CEnemy_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_LastBullet"),
		CLast_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SpawnTrigger"),
		CSpawnTrigger::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Sans"),
		CSans::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CSans_Bone"),
		CSans_Bone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CGasterLaser"),
		CGasterLaser::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CSans_Gaster"),
		CSans_Gaster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CBoneRoop"),
		CBoneRoop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CGasterRoop"),
		CGasterRoop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Beholder"),
		CBeholder::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_TrackingLaser"),
		CTrackingLaser::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Artemis"),
		CArtemis::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Apollo"),
		CApollo::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Effect_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Explosion"),
		CExplosion::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Starlight"),
		CStarLight::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Blink"),
		CBlink::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_HitBlood"),
		CHitBlood::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_HitBloodKatanaEffect"),
		CHitBloodKatanaEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_CSansMiss"),
		CSansMiss::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}


HRESULT CLoader::Loading_For_Drone_Monster()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Drone_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/Idle/air_aim%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Drone_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/Attack/air_attack%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Drone_Bound"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/Bound/Air_brain%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Drone_Death"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/Pushed/drone_flyback%d.png"), 4))))
		return E_FAIL; 

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Texture_Drone_Pushed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Pawn/Drone_Monster/Pushed/drone_flyback0.png")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Ui()
{
	if (FAILED(Ready_BossUi_Texture()))
		return E_FAIL;

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
			L"../Bin/Resources/Textures/Ui/Clear/Announcer/Announcer%d.png", 8))))
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
			L"../Bin/Resources/Textures/Ui/Clear/Shop/UpGrade/UpGrade%d.png", 3))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Ultimate_Finish_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Ultimate_Finish.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_DrinkSoda_Word_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Drink_Soda_Word.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Finish_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Finish_BackGround_Final.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_DrinkSoda_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Drink_Soda.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Damaged_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Damaged.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Blood_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/BLOOD%d.png", 6))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Execution_Show_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/CUi_Execution_Weapon.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Execution_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/CUi_Execution_BackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Execution_WeaponBackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/CUi_Execution_WeaponBackGround.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_GreenCross_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Life/CUi_GreenCross.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_FadeInOut_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Fade.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Dead_FadeOut_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/OnGoing/Dead_FadeOut.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"UI_Ultimate_Picture_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Ultimate/Ultimate_Picture.png"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_TextUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CText_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Text/TextBackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Sans_TextBack_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Text/SansTextBackGround.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Dialogue_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Dialogue/TextBackGround_Real.png"))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Dialogue_Texture",
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
	//		L"../Bin/Resources/Textures/Ui/Dialogue/Dialogue.png"))))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Ready_BossUi_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Beholder_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/BossBack_Beholder.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Artemis_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/BossBack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Apolon_BackGround_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/BossBack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_BossHpBar_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/BossHpBar%d.png", 11))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Name_Beholder_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/Name_Beholder.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Name_Artemis_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/Name_Artemis.png"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"CUi_Name_Apolon_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Beholder/Name_Apolon.png"))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Slide_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Slide/Slide.png"))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"SansFace_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/LeftArm/SansFace%d.png", 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Announcer_Phone_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Ui/Phone_Announcer/Announcer%d.png", 8))))
		return E_FAIL;


	// jeongtest
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Noise_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Noise/Noise%d.png", 2))))
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
#pragma region PistolTexture
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

#pragma endregion

#pragma region ShotgunTexture

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Shotgun_Idle_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Shotgun/Idle/SHOTGUN_IDLE%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Shotgun_Shot_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Shotgun/Shot/SHOTGUN_SHOT%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Shotgun_SpinA_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Shotgun/SpinA/SHOTGUN_A%d.png", 13))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Shotgun_SpinB_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Shotgun/SpinB/SHOTGUNB%d.png", 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Shotgun_Reload_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Gun/Shotgun/Reload/SHOTGUN_SPECIAL%d.png", 13))))
		return E_FAIL;

#pragma endregion

#pragma region Katana

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Katana_Idle_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Katana/Idle/KATANA%d.png", 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Katana_Hit_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Katana/Hit/KATANA_HIT%d.png",1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Katana_Slash_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Katana/Slash/Slash%d.png", 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Katana_Opening_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
			L"../Bin/Resources/Textures/Player/Katana/Opening/KATANA%d.png", 20))))
		return E_FAIL;

#pragma endregion



	return S_OK;
}

HRESULT CLoader::Ready_Camera_Effect_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Camera_Dash_Textures", 
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Camera/Dash/CircleLines%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Camera_Blood_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Camera/Filter/Blood_Filter.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Slash_Ultimate_Textures",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Player/Katana/Ultimate/Ultimate_Slash%d.png"
			, 27))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Grey_Filter_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Camera/Filter/Grey_Filter.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"White_Fade_Texture",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, L"../Bin/Resources/Textures/Camera/Filter/White_Fade.png"))))
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

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_GreenCross"),
		CUi_GreenCross::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CUi_Sans_Heart"),
		CUi_Sans_Heart::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Sans_TextBack"),
		eUiRenderType::Render_NonBlend,
		CUi_Sans_TextBack::Create(m_pGraphic_Device))))
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
		eUiRenderType::Render_Blend,
		CUi_Finish::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Ultimate_Finish"),
		eUiRenderType::Render_Blend,
		CUi_Ultimate_Finish::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_DrinkSoda_Word"),
		eUiRenderType::Render_Blend,
		CUi_DrinkSoda_Word::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Execution_Show"),
		eUiRenderType::Render_NonBlend,
		CUi_Execution_Show::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_GreenCrossActive"),
		eUiRenderType::Render_NonBlend,
		CUi_GreenCrossActive::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_FadeInOut"),
		eUiRenderType::Render_Blend,
		CUI_FadeInOut::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Dead_FadeOut"),
		eUiRenderType::Render_Blend,
		CUi_Dead_FadeOut::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("CUi_Sans_Black"),
		eUiRenderType::Render_NonBlend,
		CUi_Sans_Black::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("UI_Dialogue"),
		eUiRenderType::Render_Blend,
		CUI_Dialogue::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(TEXT("UI_Ultimate_Picture"),
		eUiRenderType::Render_Blend,
		CUI_UltimatePicture::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_Gun()
{
#pragma region PistolRegion
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

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload_Arm", eUiRenderType::Render_NonBlend, CPistol_Reload_Left_Arm::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Reload_Hand", eUiRenderType::Render_NonBlend, CPistol_Reload_Left_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Pistol_Opening", eUiRenderType::Render_NonBlend, CPistol_Opening::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region ShotgunRegion

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun", eUiRenderType::Render_NonBlend, CShotgun::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun_Shot", eUiRenderType::Render_NonBlend, CShotgun_Shot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun_SpinA", eUiRenderType::Render_NonBlend, CShotgun_SpinA::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun_SpinB", eUiRenderType::Render_NonBlend, CShotgun_SpinB::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun_Reload", eUiRenderType::Render_NonBlend, CShotgun_Reload::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Shotgun_Opening", eUiRenderType::Render_NonBlend, CShotgun_Opening::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region KatanaRegion

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Katana", eUiRenderType::Render_NonBlend, CKatana::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Katana_Slash", eUiRenderType::Render_NonBlend, CKatana_Slash::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Katana_Effect", eUiRenderType::Render_NonBlend, CKatana_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Katana_Opening", eUiRenderType::Render_NonBlend, CKatana_Opening::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_CrossHair", eUiRenderType::Render_NonBlend, CCrossHair::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Kick", eUiRenderType::Render_NonBlend, CKick::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Ui_Slide", eUiRenderType::Render_NonBlend, CSlide::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Ui_LifePrototype(TEXT("CShotgun_Gunfire"),
		CShotgun_Gunfire::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Active_Camera_Effect()
{
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Camera_Dash", eUiRenderType::Render_Blend, CDash_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Camera_Blood", eUiRenderType::Render_Blend, CBlood_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Slash_Ultimate", eUiRenderType::Render_Blend, CSlash_Ultimate::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Grey_Filter", eUiRenderType::Render_Blend, CGrey_Filter::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Ui_Active(L"Noise_Filter", eUiRenderType::Render_NonBlend, CNoise_Filter::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_BGM()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Boss_Eye.wav", L"Gameplay")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Stinger_Stage_End.wav", L"Stage_End")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Stage2.wav", L"Gameplay2")))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Level_Elevator_Bgm.mp3", L"Elevator_BGM")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Elevator_SFX.mp3", L"Elevator_Level_SFX")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Elevator_FX.wav", L"Elevator_FX")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Instrumental.ogg", L"Beholder_Instrument")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Extended.ogg", L"Beholder_Extend")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/BGM/Metamorphosis.wav", L"Boss_Level_Start_BGM")))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLoader::Ready_Player_Weapon_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/pistol/Pistol_Fire0.wav", L"Pistol_Fire")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/pistol/Pistol_Upgraded_Reload.wav", L"Pistol_Reload")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/shotgun/Shotgun_Fire0.wav", L"Shotgun_Fire")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/shotgun/Shotgun_Load0.wav", L"Shotgun_Opening")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/shotgun/Shotgun_Reload.wav", L"Shotgun_Reload")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/shotgun/Shotgun_Load_Spin0.wav", L"Shotgun_SpinA")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/shotgun/Shotgun_Load_Spin1.wav", L"Shotgun_SpinB")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Player_Sword_Slash0.wav", L"Katana_Slash")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Hit_Metal1.wav", L"Katana_Hit_Metal")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Cutting_Flesh.wav", L"Katana_Cutting_Flesh")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Draw_Katana.wav", L"Katana_Opening")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Air0.wav", L"Katana_Air0")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Air1.wav", L"Katana_Air1")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Air2.wav", L"Katana_Air2")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Player_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Footstep_Stone4.wav", L"Player_Footstep")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player/Player_Hit0.wav", L"Player_Damaged")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Kick_Hit0.wav", L"Player_Kick_Hit")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Drink_Soda.wav", L"Player_Soda_Drink")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Weapon_Change0.wav", L"Player_Weapon_Change")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Player_Dash0.wav", L"Player_Dash")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Player_Dash1.wav", L"Player_AirDash")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Player_Jump0.wav", L"Player_Jump")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Touch_Ground.wav", L"Player_Land")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Finish_Punch0.wav", L"Player_Execution_Punch")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Finish_Special0.wav", L"Player_Execution_Weapon")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Player_SFX/Sliding.wav", L"Player_Sliding")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Ultimate_Start1.mp3", L"Ultimate_Voice")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Haki.mp3", L"Haki")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Blood.mp3", L"Slash_Blood")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Slash_Finish.mp3", L"Slash_Finish")))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Ready_Monster_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Pistol_Shoot1.wav", L"White_Suit_Shoot")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Hit0.wav", L"White_Suit_Slashed")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Electricity_0.wav", L"White_Suit_Electrified")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/General_Death0.wav", L"White_Suit_Death")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Drone_Alert0.wav", L"Drone_Attack")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Hit1.wav", L"Drone_Slashed")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Drone_Death0.wav", L"Drone_Death")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Explosion0.wav", L"Drone_Explosion")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/bank_boss_chainsaw/Chainsaw_Attack.wav", L"Chainsaw_Jump")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Chainsaw_Attack0.wav", L"Chainsaw_Attack")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Guns/katana/Katana_Hit2.wav", L"Chainsaw_Slashed")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/General_Death4.wav", L"Chainsaw_Death")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Electricity_1.wav", L"Chainsaw_Electrified")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Enemies/Vent_Death0.wav", L"Vent_Death")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Effect_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Ambiences/SodaMachine_Pinball.wav", L"Soda_Pour")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Ambiences/Soda_Drop.wav", L"Soda_Drop")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/GUI/Trailer_Hover.wav", L"Upgrade_Hover")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/GUI/Trailer_Select.wav", L"Upgrade_Select")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Ambiences/Blood_Splatter.wav", L"Blood_Splatter")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Danger.mp3", L"Warning")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Announcer_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Announcer/Full_Life.wav", L"Full_Life")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Announcer/Congratulations.wav", L"Congratulations")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Streamer_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Streamer/Thank_You.wav", L"Streamer_Thank_You")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Voice/FirstDialogue0.wav", L"FirstDialogue0")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Voice/FirstDialogue1.wav", L"FirstDialogue1")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Voice/FirstDialogue2.wav", L"FirstDialogue2")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Voice/Sans_Voice.mp3", L"Sans_Voice")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Etc/Noise_SFX.mp3", L"ElevatorSansDialogue2")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Beholder_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Beholder_Sound/Beholder_Laser0.wav", L"Beholder_Laser0")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Beholder_Sound/Beholder_Laser1.wav", L"Beholder_Laser1")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Beholder_Sound/Beholder_Laser2.wav", L"Beholder_Laser2")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Beholder_Sound/Beholder_Bullet.wav", L"Beholder_Bullet")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Beholder_Sound/Beholder_AirBoom.wav", L"Beholder_AirBoom")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Sans_Sound()
{
	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Sans_Sound/Gaster.wav", L"Gaster")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Sans_Sound/MEGALOVANIA.wav", L"MEGALOVANIA")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Create_Sound("../Bin/Resources/Sound/Sans_Sound/Sans_Words.wav", L"Sans_Words")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_SkyBox()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SkyBox_Texture"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SkyBox"),
		CSkyBox::Create(m_pGraphic_Device))))
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
