#include "HitBloodKatanaEffect.h"

#include "GameInstance.h"

CHitBloodKatanaEffect::CHitBloodKatanaEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CHitBloodKatanaEffect::CHitBloodKatanaEffect(const CHitBloodKatanaEffect& rhs)
	: CEffect{ rhs }
{
}

HRESULT CHitBloodKatanaEffect::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, L"BloodKatanaEffect_Textures", L"HitBloodKatanaEffect")))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(2.5f, 2.5f, 1.f));

	// Play Sound;

	m_pAnimation->Play_Animation(L"HitBloodKatanaEffect", 0.1f, false);
	return S_OK;
}

HRESULT CHitBloodKatanaEffect::Add_Components()
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

CHitBloodKatanaEffect* CHitBloodKatanaEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitBloodKatanaEffect* pInstance = new CHitBloodKatanaEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHitBloodKatanaEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHitBloodKatanaEffect* CHitBloodKatanaEffect::Clone(void* pArg)
{
	CHitBloodKatanaEffect* pInstance = new CHitBloodKatanaEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHitBloodKatanaEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitBloodKatanaEffect::Free()
{
	__super::Free();
}
