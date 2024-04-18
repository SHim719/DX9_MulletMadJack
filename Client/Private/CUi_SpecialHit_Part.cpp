#include "CUi_SpecialHit_Part.h"
#include "GameInstance.h"
#include "CUi_Background.h"


CUi_SpecialHit_Part::CUi_SpecialHit_Part(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUi_SpecialHit_Part::CUi_SpecialHit_Part(const CUi_SpecialHit_Part& rhs)
	: CUi(rhs)
{
}

HRESULT CUi_SpecialHit_Part::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();
	return S_OK;
}

HRESULT CUi_SpecialHit_Part::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	m_pBackGround = (CUi_Background*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_BackGround"), pArg);
	return S_OK;
}

void CUi_SpecialHit_Part::PriorityTick(_float fTimeDelta)
{
}

void CUi_SpecialHit_Part::Tick(_float fTimeDelta)
{
}

void CUi_SpecialHit_Part::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_SpecialHit_Part::Render()
{
	m_pBackGround->Render();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUi_SpecialHit_Part::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	PartDesc* Part = (PartDesc*)pArg;
	m_UiDesc.m_fSizeX = Part->Desc.m_fSizeX;
	m_UiDesc.m_fSizeY = Part->Desc.m_fSizeY;
	m_UiDesc.m_fX = Part->Desc.m_fX;
	m_UiDesc.m_fY = Part->Desc.m_fY;
	m_UiDesc.m_Rotation = Part->Desc.m_Rotation;
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 0 };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pTransformCom->Rotation_XYZ(m_UiDesc.m_Rotation);
}

void CUi_SpecialHit_Part::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(0);
}

void CUi_SpecialHit_Part::Set_Pos(_float3 Position)
{
	m_pBackGround->Set_Pos(Position);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

void CUi_SpecialHit_Part::Set_Rotation(_float3 Rotation)
{
	m_pTransformCom->Rotation_XYZ(Rotation);
}

void CUi_SpecialHit_Part::Default_Set_LifeTime()
{
	
}

void CUi_SpecialHit_Part::Default_Set_Size()
{
}

HRESULT CUi_SpecialHit_Part::Add_Components(void* pArg)
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

HRESULT CUi_SpecialHit_Part::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_SpecialHit_Part_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	PartDesc* Part = (PartDesc*)pArg;
	m_iTexture_Index = (_uint)Part->Second;

	return S_OK;
}

CUi_SpecialHit_Part* CUi_SpecialHit_Part::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_SpecialHit_Part* pInstance = new CUi_SpecialHit_Part(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_SpecialHit_Part Prototype Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_SpecialHit_Part::Clone(void* pArg)
{
	CUi_SpecialHit_Part* pInstance = new CUi_SpecialHit_Part(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_SpecialHit_Part Clone Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_SpecialHit_Part::Free()
{
	Safe_Release(m_pBackGround);
	__super::Free();
}
