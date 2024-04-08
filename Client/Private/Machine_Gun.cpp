#include "Machine_Gun.h"

CMachine_Gun::CMachine_Gun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CMachine_Gun::CMachine_Gun(const CMachine_Gun& rhs)
	: CUi(rhs)
{
}

HRESULT CMachine_Gun::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CMachine_Gun::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);
	m_iTexture_Index = 0;


	return S_OK;
}

HRESULT CMachine_Gun::Initialize_Active()
{
	
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);
	return S_OK;
}

void CMachine_Gun::PriorityTick(_float fTimeDelta)
{
}

void CMachine_Gun::Tick(_float fTimeDelta)
{
}

void CMachine_Gun::LateTick(_float fTimeDelta)
{
}

HRESULT CMachine_Gun::Render()
{

 	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CMachine_Gun::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = 0;
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	_float3 Rotation = { 0.f, 0.f, 0.f };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Rotation_XYZ(Rotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CMachine_Gun::Initialize_Set_Speed()
{
}

void CMachine_Gun::Default_Set_LifeTime()
{
}

void CMachine_Gun::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 256 / 5.0f;
	m_UiDesc.m_fSizeY = 253 / 5.0f;
}

HRESULT CMachine_Gun::Add_Components(void* pArg)
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

	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;

	return S_OK;
}


HRESULT CMachine_Gun::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CrossHair_Textures")
		,(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CMachine_Gun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMachine_Gun* pInstance = new CMachine_Gun(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CMachine_Gun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CMachine_Gun::Free()
{
	__super::Free();
}
