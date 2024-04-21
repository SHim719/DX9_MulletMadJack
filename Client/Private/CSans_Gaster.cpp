#include "CSans_Gaster.h"

CSans_Gaster::CSans_Gaster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSans_Gaster::CSans_Gaster(const CSans_Gaster& rhs)
	:CGameObject(rhs)
{
}

HRESULT CSans_Gaster::Initialize_Prototype()
{
	return S_OK;
}
HRESULT CSans_Gaster::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_strTag = "SansBone";
	return S_OK;
}

void CSans_Gaster::PriorityTick(_float fTimeDelta)
{
}

void CSans_Gaster::Tick(_float fTimeDelta)
{
	m_fLife -= fTimeDelta;
	if (m_fLife < 0)
	{
		m_bDestroyed = true;
	}
}

void CSans_Gaster::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSans_Gaster::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	return S_OK;
}

void CSans_Gaster::OnTriggerEnter(CGameObject* pOther)
{
}

HRESULT CSans_Gaster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component
	(LEVEL_GAMEPLAY, TEXT("CSans_Bone_Texture"), TEXT("CSans_Bone_Texture")));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.25f, 0.25f, 0.5f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
	(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

CSans_Gaster* CSans_Gaster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSans_Gaster* pInstance = new CSans_Gaster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CSans_Gaster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSans_Gaster::Clone(void* pArg)
{
	CSans_Gaster* pInstance = new CSans_Gaster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create clone: CSans_Gaster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSans_Gaster::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
