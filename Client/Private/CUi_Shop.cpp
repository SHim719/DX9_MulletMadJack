#include "CUi_Shop.h"

CUi_Shop::CUi_Shop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Shop::CUi_Shop(const CUi_Shop& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Shop::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Shop::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Shop::PriorityTick(_float fTimeDelta)
{
}

void CUi_Shop::Tick(_float fTimeDelta)
{
}

void CUi_Shop::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Shop::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Shop::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return E_NOTIMPL;
}

void CUi_Shop::Initialize_Set_ActiveTime()
{

}

void CUi_Shop::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 500;
	m_UiDesc.m_fSizeY = 300;
}

void CUi_Shop::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(500);
}

void CUi_Shop::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 100.f;
	m_UiDesc.m_fY = -500.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Shop::Add_Components(void* pArg)
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

HRESULT CUi_Shop::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Shop_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Shop::Move(_float fTimeDelta)
{
}

CUi_Shop* CUi_Shop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Shop* pInstance = new CUi_Shop(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Shop Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Shop::Free()
{
	__super::Free();
}
