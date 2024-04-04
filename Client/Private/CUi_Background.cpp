#include "CUi_Background.h"

CUi_Background::CUi_Background(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi_Life(pGraphic_Device)
{
}

CUi_Background::CUi_Background(const CUi_Background& rhs)
	: CUi_Life(rhs)
{
}

HRESULT CUi_Background::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_Background::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;


	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();


	return S_OK;
}

void CUi_Background::PriorityTick(_float fTimeDelta)
{
}

void CUi_Background::Tick(_float fTimeDelta)
{

}

void CUi_Background::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Background::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Background::Add_Components(void* pArg)
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

HRESULT CUi_Background::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Background_Texture")
		,(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Background::Default_Set_LifeTime()
{
}

void CUi_Background::Default_Set_Size()
{
}

void CUi_Background::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX(TEXT("Background SizeSet Failed"));
		return;
	}

	Ui_Pos_Size_Rotation* Arg = (Ui_Pos_Size_Rotation*)pArg;
	m_UiDesc.m_fSizeX = Arg->m_fSizeX;
	m_UiDesc.m_fSizeY = Arg->m_fSizeY;
	m_UiDesc.m_fX = Arg->m_fX;
	m_UiDesc.m_fY = Arg->m_fY;
	m_UiDesc.m_Rotation = Arg->m_Rotation;


	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };


	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pTransformCom->Rotation_XYZ(m_UiDesc.m_Rotation);
}

void CUi_Background::Initialize_Set_Speed()
{
}



void CUi_Background::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

void CUi_Background::Set_Rotation(_float3 Rotation)
{
	m_pTransformCom->Rotation_XYZ(Rotation);
}



CUi_Background* CUi_Background::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Background* pInstance = new CUi_Background(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Ui_Background Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Background::Clone(void* pArg)
{
	CUi_Background* pInstance = new CUi_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Ui_Background Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CUi_Background::Free()
{
	__super::Free();
}
