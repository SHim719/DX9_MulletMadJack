#include "MapObject.h"

#include "GameInstance.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMapObject::CMapObject(const CMapObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMapObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapObject::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}

void CMapObject::PriorityTick(_float fTimeDelta)
{
}

void CMapObject::Tick(_float fTimeDelta)
{
}

void CMapObject::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CMapObject::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CMapObject::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Object_Textures"), TEXT("Object_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;



	return S_OK;
}

CMapObject* CMapObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMapObject* pInstance = new CMapObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapObject::Clone(void* pArg)
{
	CMapObject* pInstance = new CMapObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
