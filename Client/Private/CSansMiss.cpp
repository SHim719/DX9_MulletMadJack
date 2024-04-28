#include "CSansMiss.h"

CSansMiss::CSansMiss(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CSansMiss::CSansMiss(const CSansMiss& rhs)
	:CEffect(rhs)
{
}

HRESULT CSansMiss::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, L"CSansMiss_Texture", L"CSansMiss")))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 0.1f));
	
	_float3* pPos = (_float3*)pArg;
	_float3 Pos = { 0, 0, 0 };

	Pos.x = pPos->x;
	Pos.y = (pPos->y);
	Pos.z = pPos->z;

	m_pTransformCom->Set_Pos(Pos);
	m_pAnimation->Play_Animation(L"CSansMiss", 0.4f, false);
	return S_OK;
}

HRESULT CSansMiss::Add_Components()
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

CSansMiss* CSansMiss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSansMiss* pInstance = new CSansMiss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSansMiss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSansMiss* CSansMiss::Clone(void* pArg)
{
	CSansMiss* pInstance = new CSansMiss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSansMiss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSansMiss::Free()
{
	__super::Free();
}
