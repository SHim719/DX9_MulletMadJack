#include "CUi_Time_Division.h"

CUi_Time_Division::CUi_Time_Division(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Time_Division::CUi_Time_Division(const CUi_Time_Division& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Time_Division::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_Time_Division::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	return S_OK;
}

void CUi_Time_Division::PriorityTick(_float fTimeDelta)
{
}

void CUi_Time_Division::Tick(_float fTimeDelta)
{
}

void CUi_Time_Division::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Time_Division::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Time_Division::Add_Components(void* pArg)
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

	Add_Texture(pArg);

	return S_OK;
}

HRESULT CUi_Time_Division::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Time_Division_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Time_Division::Default_Set_LifeTime()
{
}

void CUi_Time_Division::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 7;
	m_UiDesc.m_fSizeY = 7;
}

void CUi_Time_Division::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = 0;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Time_Division::Initialize_Set_Speed()
{
}

void CUi_Time_Division::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

CUi_Time_Division* CUi_Time_Division::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Time_Division* pInstance = new CUi_Time_Division(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Time_Division Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Time_Division::Clone(void* pArg)
{
	CUi_Time_Division* pInstance = new CUi_Time_Division(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Time_Division Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Time_Division::Free()
{
	__super::Free();
}
