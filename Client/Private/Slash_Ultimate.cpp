#include "Slash_Ultimate.h"

CSlash_Ultimate::CSlash_Ultimate(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CSlash_Ultimate::CSlash_Ultimate(const CSlash_Ultimate& rhs)
	: CUi(rhs)
{
}

HRESULT CSlash_Ultimate::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CSlash_Ultimate::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	m_fScale = { 1280.f, 720.f, 1.f };
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CSlash_Ultimate::Initialize_Active()
{
	m_iTexture_Index = 0;
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CSlash_Ultimate::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CSlash_Ultimate::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Scale(m_fScale);
}

void CSlash_Ultimate::LateTick(_float fTimeDelta)
{
}
HRESULT CSlash_Ultimate::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CSlash_Ultimate::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.f, 0.f, 0.f));
}

void CSlash_Ultimate::Initialize_Set_Speed()
{
}



HRESULT CSlash_Ultimate::Add_Components(void* pArg)
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


HRESULT CSlash_Ultimate::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Slash_Ultimate_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CSlash_Ultimate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlash_Ultimate* pInstance = new CSlash_Ultimate(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CSlash_Ultimate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSlash_Ultimate::Free()
{
	__super::Free();
}
