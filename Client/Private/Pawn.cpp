#include "Pawn.h"
#include "CUi_Combo.h"
#include "GameInstance.h"
#include "FPS_Camera.h"


CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	return S_OK;
}

void CPawn::PriorityTick(_float fTimeDelta)
{
}

void CPawn::Tick(_float fTimeDelta)
{
}

void CPawn::LateTick(_float fTimeDelta)
{
}

HRESULT CPawn::Render()
{
	return S_OK;
}

void CPawn::Call_MonsterDieUi(eMonsterGrade Grade)
{
	_float4x4 ProjMatrix = m_pCamera->Get_Proj_Matrix();
	_float4x4 ViewMatrix = m_pCamera->Get_View_Matrix();
	_float3 MonsterPos = m_pTransformCom->Get_Pos();
	D3DXVec3TransformCoord(&MonsterPos, &MonsterPos, &ViewMatrix);
	D3DXVec3TransformCoord(&MonsterPos, &MonsterPos, &ProjMatrix);
	_float4x4 ViewPortMatrix = {};
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			ViewPortMatrix.m[i][j] = 0;
		}
	}
	ViewPortMatrix.m[0][0] = 640;
	ViewPortMatrix.m[1][1] = -360;
	ViewPortMatrix.m[2][2] = 1;
	ViewPortMatrix.m[3][0] = 640;
	ViewPortMatrix.m[3][1] = 360;
	ViewPortMatrix.m[3][2] = 0;
	ViewPortMatrix.m[3][3] = 1;
	D3DXVec3TransformCoord(&MonsterPos, &MonsterPos, &ViewPortMatrix);
	CUi_MonsterDie::MonsterDieArg Arg;
	Arg.MonsterDiePosX = MonsterPos.x;
	Arg.MonsterDiePosY = MonsterPos.y;
	Arg.MonsterGrade = Grade;

	switch (Grade)
	{
		case eMonsterGrade::High:
			CPlayer_Manager::Get_Instance()->Set_PlayerHP_Add(3.f);
			break;
		case eMonsterGrade::Middle:
			CPlayer_Manager::Get_Instance()->Set_PlayerHP_Add(2.f);
			break;
		case eMonsterGrade::Low:
			CPlayer_Manager::Get_Instance()->Set_PlayerHP_Add(1.f);
			break;
		default:
			CPlayer_Manager::Get_Instance()->Set_PlayerHP_Add(1.f);
			break;
	}

	int iCombo = CPlayer_Manager::Get_Instance()->Get_Combo();
	if (iCombo > 1) {
		if (iCombo == 2) {
			CUi_Combo::ComboDesc Combo;
			Combo.bFirstCall = true;
			Combo.iKillCount = iCombo-1;
			m_pGameInstance->Add_Ui_LifeClone(L"CUi_Combo", eUiRenderType::Render_NonBlend, &Combo);
		}
		else {
			CUi_Combo::ComboDesc Combo;
			Combo.bFirstCall = false;
			Combo.iKillCount = iCombo-1;
			m_pGameInstance->Add_Ui_LifeClone(L"CUi_Combo", eUiRenderType::Render_NonBlend, &Combo);
		}
	}

	m_pGameInstance->Add_Ui_LifeClone
	(L"CUi_MonsterDie", eUiRenderType::Render_NonBlend, &Arg);
}


void CPawn::Free()
{
	__super::Free();
}
