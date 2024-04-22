#include "BoxObject.h"

#include "GameInstance.h"


CBoxObject::CBoxObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBoxObject::CBoxObject(const CBoxObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBoxObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBoxObject::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}

void CBoxObject::PriorityTick(_float fTimeDelta)
{
}

void CBoxObject::Tick(_float fTimeDelta)
{
}

void CBoxObject::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBoxObject::Render()
{
	Begin_RenderState();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

HRESULT CBoxObject::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_BoxObj*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_BoxObject_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("BoxObject_Textures"), TEXT("BoxObject_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;


	return S_OK;
}


CBoxObject* CBoxObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoxObject* pInstance = new CBoxObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBoxObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoxObject::Clone(void* pArg)
{
	CBoxObject* pInstance = new CBoxObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBoxObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxObject::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
