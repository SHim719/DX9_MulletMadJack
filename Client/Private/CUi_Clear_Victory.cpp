#include "CUi_Clear_Victory.h"
#include "GameInstance.h"


CUi_Clear_Victory::CUi_Clear_Victory(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Clear_Victory::CUi_Clear_Victory(const CUi_Clear_Victory& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Clear_Victory::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Clear_Victory::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Clear_Victory::PriorityTick(_float fTimeDelta)
{
}

void CUi_Clear_Victory::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	if (m_fActiveTime < 0)
		m_bActive = false;

	Scaling(fTimeDelta);
}

void CUi_Clear_Victory::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Clear_Victory::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Clear_Victory::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	m_fScaleTime = 0.3f;

	return S_OK;
}

void CUi_Clear_Victory::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 2.f;
}

void CUi_Clear_Victory::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 7000;
	m_UiDesc.m_fSizeY = 750;
}

void CUi_Clear_Victory::Initialize_Set_Speed()
{
}

void CUi_Clear_Victory::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = -150.f;
	m_UiDesc.m_fY = 290.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Clear_Victory::Add_Components(void* pArg)
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

HRESULT CUi_Clear_Victory::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Clear_Victory_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Clear_Victory::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Active();
	}
	else
	{
		m_bActive = false;
	}
}

void CUi_Clear_Victory::Scaling(_float fTimeDelta)
{
	m_fScaleTime -= fTimeDelta;
	if (m_fScaleTime > 0)
	{
		_float3 Scale = m_pTransformCom->Get_Scale();
		Scale.x -= fTimeDelta * 19500;
		Scale.y -= fTimeDelta * 2000;
		m_pTransformCom->Set_Scale(Scale);
	}
	else
	{
		m_pTransformCom->Set_Scale(m_OriginScale);
	}
}

CUi_Clear_Victory* CUi_Clear_Victory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Clear_Victory* pInstance = new CUi_Clear_Victory(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Clear_Victory Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Clear_Victory::Free()
{
	__super::Free();
}
