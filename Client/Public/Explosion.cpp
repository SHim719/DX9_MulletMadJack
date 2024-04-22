#include "Explosion.h"

#include "GameInstance.h"

CExplosion::CExplosion(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CEffect { pGraphic_Device }
{
}

CExplosion::CExplosion(const CExplosion& rhs)
    : CEffect{ rhs }
{
}

HRESULT CExplosion::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, L"Explosion_Textures", L"Explosion")))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(2.f, 2.f, 1.f));

	// Play Sound;

	m_pAnimation->Play_Animation(L"Explosion", 0.07f, false);
    return S_OK;
}

HRESULT CExplosion::Add_Components()
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

CExplosion* CExplosion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExplosion* pInstance = new CExplosion(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CExplosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CExplosion* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CExplosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();
}
