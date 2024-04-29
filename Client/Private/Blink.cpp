#include "Blink.h"
#include "GameInstance.h"

CBlink::CBlink(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CBlink::CBlink(const CBlink& rhs)
	: CEffect{ rhs }
{
}

HRESULT CBlink::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, L"Blink_Textures", L"Explosion")))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(2.f, 2.f, 1.f));

	// Play Sound;

	m_pAnimation->Play_Animation(L"Explosion", 0.07f, false);
	return S_OK;
}

HRESULT CBlink::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pAnimation = dynamic_cast<CAnimation*>(Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), nullptr));
	if (nullptr == m_pAnimation)
		return E_FAIL;

	return S_OK;
}

HRESULT CBlink::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	return S_OK;
}

HRESULT CBlink::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CBlink* CBlink::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlink* pInstance = new CBlink(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlink"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBlink* CBlink::Clone(void* pArg)
{
	CBlink* pInstance = new CBlink(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBlink"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlink::Free()
{
	__super::Free();
}
