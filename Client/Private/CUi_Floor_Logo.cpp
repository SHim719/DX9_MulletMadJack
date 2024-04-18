#include "CUi_Floor_Logo.h"


CUi_Floor_Logo::CUi_Floor_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Floor_Logo::CUi_Floor_Logo(const CUi_Floor_Logo& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Floor_Logo::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Floor_Logo::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Floor_Logo::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor_Logo::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_OriginPos);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}
}

void CUi_Floor_Logo::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Floor_Logo::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Floor_Logo::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return S_OK;
}

void CUi_Floor_Logo::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Floor_Logo::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 280;
	m_UiDesc.m_fSizeY = 110;
}

void CUi_Floor_Logo::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1000);
}

void CUi_Floor_Logo::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 770.f;
	m_UiDesc.m_fY = -310.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Floor_Logo::Add_Components(void* pArg)
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

	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Floor_Logo::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Floor_Logo_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Floor_Logo::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Active();
	}
	else
	{
		Initialize_Set_ActiveTime();
	}
}

void CUi_Floor_Logo::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Speed(1300);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

CUi_Floor_Logo* CUi_Floor_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Floor_Logo* pInstance = new CUi_Floor_Logo(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Floor_Logo Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Floor_Logo::Free()
{
	__super::Free();
}
