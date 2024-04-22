#include "HitBlood.h"

#include "GameInstance.h"

CHitBlood::CHitBlood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CHitBlood::CHitBlood(const CHitBlood& rhs)
	: CEffect{ rhs }
{
}

HRESULT CHitBlood::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, L"HitBlood_Textures", L"HitBlood")))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));

	// Play Sound;

	m_pAnimation->Play_Animation(L"HitBlood", 0.1f, false);
	return S_OK;
}

HRESULT CHitBlood::Add_Components()
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

CHitBlood* CHitBlood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitBlood* pInstance = new CHitBlood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHitBlood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHitBlood* CHitBlood::Clone(void* pArg)
{
	CHitBlood* pInstance = new CHitBlood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHitBlood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitBlood::Free()
{
	__super::Free();
}
