#include "CUi_Floor_Part.h"
#include "GameInstance.h"


CUi_Floor_Part::CUi_Floor_Part(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi_Floor(pGraphic_Device)
{
}

CUi_Floor_Part::CUi_Floor_Part(const CUi_Floor_Part& rhs)
	:CUi_Floor(rhs)
{
}

HRESULT CUi_Floor_Part::Initialize_Prototype()
{
	Default_Set_LifeTime();

	return S_OK;
}

HRESULT CUi_Floor_Part::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_SetNumber_LifeTime();
	Initialize_Set_Scale_Pos_Rotation(pArg);

	return S_OK;
}

void CUi_Floor_Part::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor_Part::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	Execute(fTimeDelta);
	if (m_fLifeTime <= 0)
	{
		Set_Dead();
	}
}

void CUi_Floor_Part::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Floor_Part::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUi_Floor_Part::Default_Set_LifeTime()
{
	m_fLifeTime = 5.f;
}

void CUi_Floor_Part::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 80;
	m_UiDesc.m_fSizeY = 80;
}


void CUi_Floor_Part::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	switch (m_iPart)
	{
	case (_uint)Part::Number1:
		m_UiDesc.m_fX = -50;
		m_UiDesc.m_fY = 70;
		m_fRotationTime = 0;
		break;
	case (_uint)Part::Number2:
		m_UiDesc.m_fX = +50;
		m_UiDesc.m_fY = 70;
		m_fRotationTime = 0;
		break;
	default:
		m_UiDesc.m_fX = -430 + (_float)(m_iPart * 100);
		m_UiDesc.m_fY = 400;
		m_fRotationTime = 0;
		break;
	}
	m_fRotationTimeEnd = 1.5f;
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 1.f));
}

void CUi_Floor_Part::Initialize_Set_Speed()
{
	if (m_iPart == (_uint)Part::Number1
		|| m_iPart == (_uint)Part::Number2)
	{
		m_pTransformCom->Set_Speed(0);
	}
	else
	{
		m_fSpeed = 600;
		m_pTransformCom->Set_Speed(m_fSpeed);
	}
}

void CUi_Floor_Part::Initialize_SetNumber_LifeTime()
{
	switch (m_iPart)
	{
	case (_uint)Part::F:
		m_fLifeTime = 5.5f;
		break;
	case (_uint)Part::L:
		m_fLifeTime = 5.4f;
		break;
	case (_uint)Part::First_O:
		m_fLifeTime = 5.3f;
		break;
	case (_uint)Part::Second_O:
		m_fLifeTime = 5.2f;
		break;
	case (_uint)Part::R:
		m_fLifeTime = 5.1f;
		break;
	case (_uint)Part::Number1:
		m_fLifeTime = 2.5f;
		break;
	case (_uint)Part::Number2:
		m_fLifeTime = 2.4f;
		break;
	default:
		break;
	}
}

HRESULT CUi_Floor_Part::Add_Components(void* pArg)
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

HRESULT CUi_Floor_Part::Add_Texture(void* pArg)
{
	Part* ePart = (Part*)(pArg);
	m_iPart = _uint(*ePart);
	if (*ePart == Part::F)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_F_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::L)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_L_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::First_O)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_O_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::Second_O)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_O_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::R)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_R_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::Number1)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_Floor_0_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (*ePart == Part::Number2)
	{
		int temp = 0;
		temp = m_pGameInstance->Get_CurrentLevelID();
		temp -= (_uint)LEVEL::LEVEL_GAMEPLAY;
		Set_NumberTexture(temp);
	}
	return S_OK;
}

HRESULT CUi_Floor_Part::Set_NumberTexture(_uint LevelId)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Floor_0_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	m_iTexture_Index = LevelId;

	return S_OK;
}

void CUi_Floor_Part::Execute(_float fTimeDelta)
{
	m_fMoveTime -= fTimeDelta;
	if (m_iPart != (_uint)Part::Number1
		&& m_iPart != (_uint)Part::Number2)
	{
		if (m_fMoveTime > 0)
		{
			Sub_Speed(fTimeDelta);
			Move(fTimeDelta);
			Rotation(fTimeDelta);
		}
		else if(m_fMoveTime <= 0 && m_fLifeTime > 2.5)
		{
			Set_Regular_Pos();
		}

		if (m_fLifeTime < 2.5 && 0 < m_fLifeTime)
		{
			m_pTransformCom->Set_Speed(1500);
			Finale(fTimeDelta);
		}
	}
}

void CUi_Floor_Part::Sub_Speed(_float fTimeDelta)
{
	if (m_fSpeed < 0)
	{
		m_fSpeed = 0;
	}
	m_pTransformCom->Set_Speed(m_fSpeed);
}

void CUi_Floor_Part::Move(_float fTimeDelta)
{
	m_fSpeed -= 600 * fTimeDelta;
	if (!m_bReverse)
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	
}

void CUi_Floor_Part::Rotation(_float fTimeDelta)
{
	m_fRotationTime += fTimeDelta;
	if (m_fRotationTime > 0.07)
	{
		m_UiDesc.m_fSizeX -= 7;
		if (m_UiDesc.m_fSizeX < 0 && !m_bReverse)
		{
			m_bReverse = !m_bReverse;
			_float3 Reverse = { 0, 180, 0 };
			m_pTransformCom->Rotation_XYZ(Reverse);
			++m_iTexture_Index;
		}
		
		if (m_bReverse)
		{
			_float3 Scale = { -m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1 };
			m_pTransformCom->Set_Scale(Scale);
		}
		else
		{
			_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1 };
			m_pTransformCom->Set_Scale(Scale);
		}
	}

}

void CUi_Floor_Part::Set_Regular_Pos()
{
	_float3 RegularPos = { m_OriginPos.x + 100 * m_iPart, m_OriginPos.y, 1.f };
	m_pTransformCom->Set_Position(RegularPos);
}

void CUi_Floor_Part::Finale(_float fTimeDelta)
{
	switch (m_iPart)
	{
	case (_uint)Part::F:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	case (_uint)Part::L:
		m_pTransformCom->Go_Right(fTimeDelta);
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case (_uint)Part::First_O:
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case (_uint)Part::Second_O:
		m_pTransformCom->Go_Left(fTimeDelta);
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case (_uint)Part::R:
		m_pTransformCom->Go_Left(fTimeDelta);
		break;
	default:
		break;
	}
}

CUi_Floor_Part* CUi_Floor_Part::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Floor_Part* pInstance = new CUi_Floor_Part(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Floor_Part Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Floor_Part::Clone(void* pArg)
{
	CUi_Floor_Part* pInstance = new CUi_Floor_Part(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Floor_Part Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Floor_Part::Free()
{
	__super::Free();
}


