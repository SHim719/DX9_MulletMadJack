#include "CUi_MonsterDie.h"
#include "GameInstance.h"
#include "CUi_Background.h"


CUi_MonsterDie::CUi_MonsterDie(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUi_MonsterDie::CUi_MonsterDie(const CUi_MonsterDie& rhs)
	: CUi(rhs)
{
}

HRESULT CUi_MonsterDie::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();
	return S_OK;
}

HRESULT CUi_MonsterDie::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_MoveLogic_Select();
	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();
	Initialize_Set_Background();

	return S_OK;
}

void CUi_MonsterDie::PriorityTick(_float fTimeDelta)
{
}

void CUi_MonsterDie::Tick(_float fTimeDelta)
{
	Move(fTimeDelta);
	Cal_Life_Blink(fTimeDelta);
	
}

void CUi_MonsterDie::LateTick(_float fTimeDelta)
{
	m_pBackGround->Set_Pos(m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION));
}

HRESULT CUi_MonsterDie::Render()
{
	if (Cal_BlinkRender(0.1f))
	{
		m_pBackGround->Render();
		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;
		m_pTextureCom->Bind_Texture(m_iTexture_Index);
		m_pVIBufferCom->Render();	
	}
	return S_OK;
}

HRESULT CUi_MonsterDie::Add_Components(void* pArg)
{
	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;


	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;
	

	return S_OK;
}



HRESULT CUi_MonsterDie::Add_Texture(void* pArg)
{
	auto Arg = (MonsterDie_Arg*)pArg;
	eMonsterGrade Grade = Arg->MonsterGrade;
	if (Grade == eMonsterGrade::Low)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_MonsterLowGrade_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (Grade == eMonsterGrade::Middle)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_MonsterMiddleGrade_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (Grade == eMonsterGrade::High)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_MonsterHighGrade_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	return S_OK;
}

void CUi_MonsterDie::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 128;
	m_UiDesc.m_fSizeY = 32;
}

void CUi_MonsterDie::Default_Set_LifeTime()
{
	m_fLifeTime = 2.f;
}


void CUi_MonsterDie::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	if (pArg == nullptr)
	{
		m_UiDesc.m_fX = g_iWinSizeX * 0.5f;
		m_UiDesc.m_fY = 500.f;
	}
	else
	{
		auto Position = (MonsterDie_Arg*)pArg;
		m_UiDesc.m_fX = Position->MonsterDiePosX;
		m_UiDesc.m_fY = Position->MonsterDiePosY;
		m_UiDesc.m_Rotation = Position->MonsterRotation;
		//if (rand() % 2)
		//	m_UiDesc.m_fY = Position->MonsterDiePosY;
		//else
		//	m_UiDesc.m_fY = 100.f;
	}

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_pTransformCom->Set_Scale(Scale*1.25);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	if (m_UiDesc.m_fX >= 0)
	{
		m_bStartPosRight = true;
	}
	else
	{
		m_bStartPosRight = false;
	}
}

void CUi_MonsterDie::Initialize_Set_Speed()
{
	switch (m_eMoveLogic)
	{
	case eMoveLogic::Slow:
		m_pTransformCom->Set_Speed(450);
		break;
	case eMoveLogic::Fast:
		m_pTransformCom->Set_Speed(1000);
		break;
	case eMoveLogic::Bound:
		m_pTransformCom->Set_Speed(600);
		break;
	case eMoveLogic::Random:
		m_pTransformCom->Set_Speed(700);
		break;
	default:
		break;
	}
}

void CUi_MonsterDie::Initialize_MoveLogic_Select()
{
 	_uint temp = (rand() % (_uint)eMoveLogic::End);
	switch (temp)
	{
	case (_uint)eMoveLogic::Slow:
		m_eMoveLogic = eMoveLogic::Slow;
		break;
	case (_uint)eMoveLogic::Fast:
		m_eMoveLogic = eMoveLogic::Fast;
		break;
	case (_uint)eMoveLogic::Bound:
		m_eMoveLogic = eMoveLogic::Bound;
		break;
	case (_uint)eMoveLogic::Random:
		m_eMoveLogic = eMoveLogic::Random;
		break;
	default:
		break;
	}
}

void CUi_MonsterDie::Initialize_Set_Background()
{
	m_pBackGround = (CUi_Background*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_BackGround"), &m_UiDesc);
}
 
void CUi_MonsterDie::Move(_float fTimeDelta)
{
	Adjust_YPos(fTimeDelta);

	switch (m_eMoveLogic)
	{
	case eMoveLogic::Slow:
		Move_Slow(fTimeDelta);
		break;
	case eMoveLogic::Fast:
		Move_Fast(fTimeDelta);
		break;
	case eMoveLogic::Bound:
		Move_Bound(fTimeDelta);
		break;
	case eMoveLogic::Random:
		Move_Random(fTimeDelta);
		break;
	default:
		break;
	}
}

void CUi_MonsterDie::Teleport()
{
	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	if (temp.x < -(_int)UIPOS::ScreenXpos)
	{
		temp.x = (_int)UIPOS::ScreenXpos;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &temp);
	}
	
	if (temp.x > (_int)UIPOS::ScreenXpos)
	{
		temp.x = -(_int)UIPOS::ScreenXpos;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &temp);
	}
}

void CUi_MonsterDie::Move_Slow(_float fTimeDelta)
{
	if (m_bStartPosRight)
	{
		m_pTransformCom->AddSpeedPerSec(-200.f * fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);

	}
	else
	{
		m_pTransformCom->AddSpeedPerSec(-200.f * fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);

	}
}

void CUi_MonsterDie::Move_Fast(_float fTimeDelta)
{
	Teleport();
	if (m_bStartPosRight)
	{
		m_pTransformCom->AddSpeedPerSec(-300.f * fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);

	}
	else
	{
		m_pTransformCom->AddSpeedPerSec(-300.f * fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);

	}
}

void CUi_MonsterDie::Move_Bound(_float fTimeDelta)
{
	if (m_bStartPosRight)
	{
		m_pTransformCom->AddSpeedPerSec(-100.f * fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->AddSpeedPerSec(-100.f * fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	if (temp.x >((_int)UIPOS::ScreenXpos - m_UiDesc.m_fSizeX))
	{
		m_bStartPosRight = !m_bStartPosRight;
		m_pTransformCom->Set_Speed(1000);
	}
	else if (temp.x<(-(_int)UIPOS::ScreenXpos + m_UiDesc.m_fSizeX))
	{
		m_pTransformCom->Set_Speed(1000);
		m_bStartPosRight = !m_bStartPosRight;
	}
}

void CUi_MonsterDie::Move_Random(_float fTimeDelta)
{
	m_fRandomLogicTimeGap += fTimeDelta;
	if (m_bStartPosRight)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);

	if (temp.x > ((_int)UIPOS::ScreenXpos - m_UiDesc.m_fSizeX))
	{
		m_bStartPosRight = !m_bStartPosRight;
	}
	else if (temp.x < (-(_int)UIPOS::ScreenXpos + m_UiDesc.m_fSizeX))
	{
		m_pTransformCom->Set_Speed(1000);
	}

	if (m_fRandomLogicTimeGap > 0.5)
	{
		Random_Speed();
		Random_Direction();
		m_fRandomLogicTimeGap = 0.f;
	}
	
}

void CUi_MonsterDie::Adjust_YPos(_float fTimeDelta)
{

	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	if (Pos.y < (_uint)UIPOS::MonsterDieYpos - 10)
	{
		m_pTransformCom->MonsterDieUi_Go_Up(fTimeDelta, 2500);
	}
	else if (Pos.y > (_uint)UIPOS::MonsterDieYpos + 10)
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}
	else
	{
		if (!m_bScale)
		{
			_float3 Scale = { 0.8f, 0.8f, 0.f };
			m_bScale = !m_bScale;
			m_pTransformCom->Set_Scale(Scale);
		}	
		return;
	}
}

void CUi_MonsterDie::Random_Direction()
{
	_int Direction = rand() % 2;
	if (Direction)
	{
		m_bStartPosRight = true;
	}
	else
	{
		m_bStartPosRight = false;
	}
}

void CUi_MonsterDie::Random_Speed()
{
	_int Speed = rand() % 1000;
	m_pTransformCom->Set_Speed((_float)Speed);
}

void CUi_MonsterDie::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}



CUi_MonsterDie* CUi_MonsterDie::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_MonsterDie* pInstance = new CUi_MonsterDie(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_MonsterDie Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_MonsterDie::Clone(void* pArg)
{
	CUi_MonsterDie* pInstance = new CUi_MonsterDie(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_MonsterDie_Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_MonsterDie::Free()
{
	Safe_Release(m_pBackGround);

	__super::Free();
}