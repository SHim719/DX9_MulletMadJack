#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"
#include "CUi_PEACE.h"
#include "Level_Loading.h"
#include "CGame_Manager.h"
#include "Enemy.h"
#include "Enemy_Bullet.h"

#include "Wall.h"
#include "CrossHair.h"
#include "Core_Camera.h"
#include "CUi_SpecialHit.h"
#include "CUi_MonsterDie.h"
#include "PlayerManager.h"
#include "Player.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;

	Load_MapObject(L"../Bin/Resources/DataFiles/TestMap.dat", OBJTYPE_END);

	if (FAILED(Ready_Layer_Camera(TEXT("Main_Camera"))))
		return E_FAIL;

	if(FAILED(Ready_Layer_Player()))
		return E_FAIL;
	
	//if (FAILED(Test_UiTexture_Loading()))
	//	return E_FAIL;

	/*if (FAILED(Test_LifeUi_Clone()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Enemy(TEXT("Layer_Enemy"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::U))
	{
		CGame_Manager::Get_Instance()->Set_StageProgress(CGame_Manager::StageProgress::Clear);
	}
	else if (m_pGameInstance->GetKeyDown(eKeyCode::I))
	{
		CGame_Manager::Get_Instance()->Set_StageProgress(CGame_Manager::StageProgress::Start);
	}
	
}

HRESULT CLevel_GamePlay::Render()
{

	SetWindowText(g_hWnd, TEXT("게임플레이레벨."));

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

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

	CameraDesc.vEye = _float3(0.f, 0.65f, 1.7f);
	CameraDesc.vAt = _float3(0.5f, 0.5f, 10.f);
	CameraDesc.fFovy = D3DXToRadian(90.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.0f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Create_Camera(strLayerTag, CFPS_Camera::Create(m_pGraphic_Device, &CameraDesc))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Enemy(const wstring& strLayerTag)
{
	CEnemy::ENEMY_DESC	EnemyDesc{};

	//EnemyDesc.vPosition = _float3(5.f, 0.f, 5.f);
	//EnemyDesc.vAt = _float3(pFPS_Camera->Get_CAMERA_DESC().vEye);
	//EnemyDesc.fSpeedPerSec = 2.f;
	//EnemyDesc.fRotationPerSec = D3DXToRadian(90.f);
	//
	//if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Enemy"), &EnemyDesc))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	if(CPlayer_Manager::Get_Instance()->Get_Player())
		return E_FAIL;

	CPlayer_Manager::Get_Instance()->Set_Player(dynamic_cast<CPlayer*>(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Player"), TEXT("Prototype_Player"))));

	if (nullptr == CPlayer_Manager::Get_Instance()->Get_Player()) {
		MSG_BOX(TEXT("Failed to Create Player"));
		return E_FAIL;
	}

	return S_OK;
}

void CLevel_GamePlay::Free()
{

	__super::Free();

}

HRESULT CLevel_GamePlay::Test_LifeUi_Clone()
{
	/*CUi_MonsterDie::MonsterDie_Arg Arg;
	Arg.MonsterDiePosX = -200;
	Arg.MonsterDiePosY = -200;
	Arg.MonsterGrade = eMonsterGrade::High;
	if (FAILED(m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_MonsterDie"),
		eUiRenderType::Render_Blend,
		&Arg)))
		return E_FAIL;*/


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
	//CUi_Peace_Texture

	return S_OK;
}