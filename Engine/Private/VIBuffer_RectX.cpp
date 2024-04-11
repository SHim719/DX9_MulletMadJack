#include "VIBuffer_RectX.h"

CVIBuffer_RectX::CVIBuffer_RectX(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer_Rect{ pGraphic_Device }
{
}

CVIBuffer_RectX::CVIBuffer_RectX(const CVIBuffer_Rect& rhs)
	: CVIBuffer_Rect{ rhs }
{
}

HRESULT CVIBuffer_RectX::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_RectX::Initialize(void* pArg)
{
	Safe_Release(m_pVB);

	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 2;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */

	m_vecPositions.resize(4);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);
	pVertices[0].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);
	pVertices[1].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[2].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[3].vNormal = _float3(0.0f, 0.f, -1.f);

	m_pVB->Unlock();

	m_vecPositions[0] = pVertices[0].vPosition;
	m_vecPositions[1] = pVertices[1].vPosition;
	m_vecPositions[2] = pVertices[2].vPosition;
	m_vecPositions[3] = pVertices[3].vPosition;

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();
	return S_OK;
}

void CVIBuffer_RectX::Scaling_Texcoord(const _float3& vScale)
{
	if (vScale.x < 1.f)
		return;

	VTXNORMAL* pVertices = { nullptr };

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[1].vTexcoord = _float2(vScale.x, 0.f);
	pVertices[2].vTexcoord = _float2(vScale.x, 1.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	m_pVB->Unlock();
}

CVIBuffer_RectX* CVIBuffer_RectX::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RectX* pInstance = new CVIBuffer_RectX(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_RectX"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RectX::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_RectX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_RectX"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
