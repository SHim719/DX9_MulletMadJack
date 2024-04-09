#include "CUi_Shop_Noise_Part.h"

CUi_Shop_Noise_Part::CUi_Shop_Noise_Part(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Shop_Noise_Part::CUi_Shop_Noise_Part(const CUi_Shop_Noise_Part& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Shop_Noise_Part::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_Shop_Noise_Part::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	return S_OK;
}

void CUi_Shop_Noise_Part::PriorityTick(_float fTimeDelta)
{
}

void CUi_Shop_Noise_Part::Tick(_float fTimeDelta)
{
}

void CUi_Shop_Noise_Part::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Shop_Noise_Part::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Shop_Noise_Part::Add_Components(void* pArg)
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

	Add_Texture(nullptr);

	return S_OK;
}

HRESULT CUi_Shop_Noise_Part::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Shop_NoisePart_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Shop_Noise_Part::Default_Set_LifeTime()
{
	m_fLifeTime = 0;
}

void CUi_Shop_Noise_Part::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 500;
	m_UiDesc.m_fSizeY = 300;
}

void CUi_Shop_Noise_Part::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX(TEXT("CUi_Shop_Noise_Part SizeSet Failed"));
		return;
	}

	Ui_Pos_Size_Rotation* Arg = (Ui_Pos_Size_Rotation*)pArg;
	m_UiDesc.m_fX = Arg->m_fX;
	m_UiDesc.m_fY = Arg->m_fY;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Shop_Noise_Part::Initialize_Set_Speed()
{
}

void CUi_Shop_Noise_Part::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

void CUi_Shop_Noise_Part::Set_Rotation(_float3 Rotation)
{
	m_pTransformCom->Rotation_XYZ(Rotation);
}

void CUi_Shop_Noise_Part::Set_Scale(_float3 Scale)
{

}

CUi_Shop_Noise_Part* CUi_Shop_Noise_Part::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Shop_Noise_Part* pInstance = new CUi_Shop_Noise_Part(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Shop_Noise_Part Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Shop_Noise_Part::Clone(void* pArg)
{
	CUi_Shop_Noise_Part* pInstance = new CUi_Shop_Noise_Part(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Shop_Noise_Part Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Shop_Noise_Part::Free()
{
	__super::Free();
}
