#include "VIBuffer_BoxObj.h"

CVIBuffer_BoxObj::CVIBuffer_BoxObj(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_BoxObj::CVIBuffer_BoxObj(const CVIBuffer_BoxObj& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_BoxObj::Initialize_Prototype()
{
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 12;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */

	m_vecPositions.resize(8);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vecPositions[0] = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vNormal = _float3(-1.f, 1.f, -1.f);
	D3DXVec3Normalize(&pVertices[0].vNormal, &pVertices[0].vNormal);
	pVertices[0].vTexcoord = _float2(0.5f, 0.5f);

	pVertices[1].vPosition = m_vecPositions[1] = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vNormal = _float3(1.f, 1.f, -1.f);
	D3DXVec3Normalize(&pVertices[1].vNormal, &pVertices[1].vNormal);
	pVertices[1].vTexcoord = _float2(1.f, 0.5f);

	pVertices[2].vPosition = m_vecPositions[2] = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vNormal = _float3(1.f, -1.f, -1.f);
	D3DXVec3Normalize(&pVertices[2].vNormal, &pVertices[2].vNormal);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = m_vecPositions[3] = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vNormal = _float3(-1.f, -1.f, -1.f);
	D3DXVec3Normalize(&pVertices[3].vNormal, &pVertices[3].vNormal);
	pVertices[3].vTexcoord = _float2(0.5f, 1.f);

	pVertices[4].vPosition = m_vecPositions[4] = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vNormal = _float3(-1.f, 1.f, 1.f);
	D3DXVec3Normalize(&pVertices[4].vNormal, &pVertices[4].vNormal);
	pVertices[4].vTexcoord = _float2(0.f, 0.f);

	pVertices[5].vPosition = m_vecPositions[5] = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vNormal = _float3(1.f, 1.f, 1.f);
	D3DXVec3Normalize(&pVertices[5].vNormal, &pVertices[5].vNormal);
	pVertices[5].vTexcoord = _float2(0.5f, 0.f);

	pVertices[6].vPosition = m_vecPositions[6] = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vNormal = _float3(-1.f, 1.f, 1.f);
	D3DXVec3Normalize(&pVertices[6].vNormal, &pVertices[6].vNormal);
	pVertices[6].vTexcoord = _float2(0.5f, 0.5f);

	pVertices[7].vPosition = m_vecPositions[7] = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vNormal = _float3(-1.f, -1.f, 1.f);
	D3DXVec3Normalize(&pVertices[7].vNormal, &pVertices[7].vNormal);
	pVertices[7].vTexcoord = _float2(0.0f, 0.5f);

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_BoxObj::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_BoxObj* CVIBuffer_BoxObj::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BoxObj* pInstance = new CVIBuffer_BoxObj(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_BoxObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_BoxObj::Clone(void* pArg)
{
	CVIBuffer_BoxObj* pInstance = new CVIBuffer_BoxObj(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_BoxObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_BoxObj::Free()
{
	__super::Free();

}
