#include "Border.h"

#include "GameInstance.h"



CBorder::CBorder(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBorder::CBorder(const CBorder& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBorder::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBorder::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}

void CBorder::PriorityTick(_float fTimeDelta)
{
}

void CBorder::Tick(_float fTimeDelta)
{
}

void CBorder::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBorder::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBuffer->Render();


	return S_OK;
}

HRESULT CBorder::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer = dynamic_cast<CVIBuffer_Floor_Border*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Floor_Border_Default"), TEXT("VIBuffer"), this));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Border_Textures"), TEXT("Border_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

CBorder* CBorder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBorder* pInstance = new CBorder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBorder"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBorder::Clone(void* pArg)
{
	CBorder* pInstance = new CBorder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBorder"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBorder::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTextureCom);
}

