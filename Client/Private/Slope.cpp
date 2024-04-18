#include "Slope.h"

#include "GameInstance.h"


CSlope::CSlope(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSlope::CSlope(const CSlope& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSlope::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSlope::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_strTag = "Slope";

	return S_OK;
}

void CSlope::PriorityTick(_float fTimeDelta)
{
}

void CSlope::Tick(_float fTimeDelta)
{

}

void CSlope::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSlope::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSlope::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_RectXY*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_RectXY_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Slope_Textures"), TEXT("Slope_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

CSlope* CSlope::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlope* pInstance = new CSlope(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSlope"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSlope::Clone(void* pArg)
{
	CSlope* pInstance = new CSlope(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSlope"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlope::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

}
