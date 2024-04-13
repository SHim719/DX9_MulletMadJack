#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Transform.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CRenderer::Initialize()
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderObjects(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);	

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Clear()
{
	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}
}

void CRenderer::Sort_AlphaBlendObj()
{
	if (0 == m_RenderObjects[RENDER_BLEND].size())
		return;

	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3 vCamPos;
	memcpy(&vCamPos, &ViewMatrix.m[3], sizeof(_float3));

	m_RenderObjects[RENDER_BLEND].sort([&](CGameObject* pLeft, CGameObject* pRight)
		{
			_float3 vLeftPos = pLeft->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			_float3 vRightPos = pRight->Get_Transform()->Get_State(CTransform::STATE_POSITION);

			_float vLeftLength = D3DXVec3Length(&(vCamPos - vLeftPos));
			_float vRightLength = D3DXVec3Length(&(vCamPos - vRightPos));

			return vLeftLength < vRightLength;
		});
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	for (auto& RenderObjects : m_RenderObjects)
	{		
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}
	
	Safe_Release(m_pGraphic_Device);
}
