#include "CUi_Floor_Logo_Num.h"
#include "GameInstance.h"


CUi_Floor_Logo_Num::CUi_Floor_Logo_Num(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Floor_Logo_Num::CUi_Floor_Logo_Num(const CUi_Floor_Logo_Num& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Floor_Logo_Num::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	Default_Set_LifeTime();
	Default_Set_Size();

	 
	return S_OK;
}

HRESULT CUi_Floor_Logo_Num::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	return S_OK;
}

void CUi_Floor_Logo_Num::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor_Logo_Num::Tick(_float fTimeDelta)
{
}

void CUi_Floor_Logo_Num::LateTick(_float fTimeDelta)
{

}

HRESULT CUi_Floor_Logo_Num::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUi_Floor_Logo_Num::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	Ui_Pos_Size_Rotation* Arg = (Ui_Pos_Size_Rotation*)pArg;
	m_UiDesc.m_fX = Arg->m_fX;
	m_UiDesc.m_fY = Arg->m_fY;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Floor_Logo_Num::Initialize_Set_Speed()
{
}

HRESULT CUi_Floor_Logo_Num::Add_Components(void* pArg)
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

HRESULT CUi_Floor_Logo_Num::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Floor_Logo_Num_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Floor_Logo_Num::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 40;
	m_UiDesc.m_fSizeY = 40;
}

void CUi_Floor_Logo_Num::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}
void CUi_Floor_Logo_Num::Set_LevelTexture()
{
	m_iTexture_Index = m_pGameInstance->Get_CurrentLevelID() - (_uint)LEVEL::LEVEL_GAMEPLAY;
}

CUi_Floor_Logo_Num* CUi_Floor_Logo_Num::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Floor_Logo_Num* pInstance = new CUi_Floor_Logo_Num(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Floor_Logo_Num Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Floor_Logo_Num::Clone(void* pArg)
{
	CUi_Floor_Logo_Num* pInstance = new CUi_Floor_Logo_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Floor_Logo_Num Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Floor_Logo_Num::Free()
{
	__super::Free();
}
