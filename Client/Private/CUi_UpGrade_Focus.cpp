#include "CUi_UpGrade_Focus.h"


CUi_UpGrade_Focus::CUi_UpGrade_Focus(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_UpGrade_Focus::CUi_UpGrade_Focus(const CUi_UpGrade_Focus& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_UpGrade_Focus::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_UpGrade_Focus::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	return S_OK;
}

void CUi_UpGrade_Focus::PriorityTick(_float fTimeDelta)
{
}

void CUi_UpGrade_Focus::Tick(_float fTimeDelta)
{
}

void CUi_UpGrade_Focus::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_UpGrade_Focus::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_UpGrade_Focus::Add_Components(void* pArg)
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

HRESULT CUi_UpGrade_Focus::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_UpGrade_Focus_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_UpGrade_Focus::Default_Set_LifeTime()
{
	m_fLifeTime = 0;
}

void CUi_UpGrade_Focus::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 260;
	m_UiDesc.m_fSizeY = 420;
}

void CUi_UpGrade_Focus::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3* Arg = (_float3*)pArg;
	m_UiDesc.m_fX = Arg->x;
	m_UiDesc.m_fY = Arg->y;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_UpGrade_Focus::Initialize_Set_Speed()
{

}

void CUi_UpGrade_Focus::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

CUi_UpGrade_Focus* CUi_UpGrade_Focus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_UpGrade_Focus* pInstance = new CUi_UpGrade_Focus(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_UpGrade_Focus Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_UpGrade_Focus::Clone(void* pArg)
{
	CUi_UpGrade_Focus* pInstance = new CUi_UpGrade_Focus(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_UpGrade_Focus Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_UpGrade_Focus::Free()
{
	__super::Free();
}
