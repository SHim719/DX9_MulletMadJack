#include "CText_BackGround.h"

CText_BackGround::CText_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CText_BackGround::CText_BackGround(const CText_BackGround& rhs)
	:CUi(rhs)
{
}

HRESULT CText_BackGround::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CText_BackGround::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	return S_OK;
}

void CText_BackGround::PriorityTick(_float fTimeDelta)
{
}

void CText_BackGround::Tick(_float fTimeDelta)
{
}

void CText_BackGround::LateTick(_float fTimeDelta)
{
}

HRESULT CText_BackGround::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CText_BackGround::Add_Components(void* pArg)
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

HRESULT CText_BackGround::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CText_BackGround_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CText_BackGround::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Ui_Pos_Size_Rotation* Arg = (Ui_Pos_Size_Rotation*)pArg;
	m_UiDesc.m_fSizeX = Arg->m_fSizeX;
	m_UiDesc.m_fSizeY = Arg->m_fSizeY;
	m_UiDesc.m_fX = Arg->m_fX;
	m_UiDesc.m_fY = Arg->m_fY;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CText_BackGround::Initialize_Set_Speed()
{
}

void CText_BackGround::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

CText_BackGround* CText_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CText_BackGround* pInstance = new CText_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CText_BackGround Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CText_BackGround::Clone(void* pArg)
{
	CText_BackGround* pInstance = new CText_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CText_BackGround Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CText_BackGround::Free()
{
	__super::Free();
}
