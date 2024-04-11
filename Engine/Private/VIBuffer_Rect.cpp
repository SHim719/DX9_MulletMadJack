#include "..\Public\VIBuffer_Rect.h"
#include "Transform.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer { rhs }
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXTEX);
	m_FVF = D3DFVF_XYZ | D3DFVF_TEX1; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 2;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*			pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

	m_pVB->Unlock();
	
	m_vecPositions.resize(4);
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

HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	return S_OK;
}

bool CVIBuffer_Rect::Intersect_Ray(CTransform* pTransform, const _float3& vRayWorldPos, const _float3& vRayDir, OUT _float3* pHitWorldPos, OUT _float* pDist)
{
	_float4x4 Inverse_WorldMatrix = pTransform->Get_WorldMatrix_Inverse();
	_float3 vRayLocalPos = *D3DXVec3TransformCoord(&_float3(), &vRayWorldPos, &Inverse_WorldMatrix);
	_float3 vRayLocalDir = *D3DXVec3TransformNormal(&_float3(), &vRayDir, &Inverse_WorldMatrix);

	_float fU, fV;
	if (D3DXIntersectTri(&m_vecPositions[0], &m_vecPositions[1], &m_vecPositions[2], &vRayLocalPos, &vRayLocalDir, &fU, &fV, pDist))
	{
		*pHitWorldPos = m_vecPositions[0] + fU * (m_vecPositions[1] - m_vecPositions[0]) + fV * (m_vecPositions[2] - m_vecPositions[0]);
		D3DXVec3TransformCoord(pHitWorldPos, pHitWorldPos, &pTransform->Get_WorldMatrix());
		return true;
	}

	if (D3DXIntersectTri(&m_vecPositions[0], &m_vecPositions[2], &m_vecPositions[3], &vRayLocalPos, &vRayLocalDir, &fU, &fV, pDist))
	{
		*pHitWorldPos = m_vecPositions[0] + fU * (m_vecPositions[2] - m_vecPositions[0]) + fV * (m_vecPositions[3] - m_vecPositions[0]);
		D3DXVec3TransformCoord(pHitWorldPos, pHitWorldPos, &pTransform->Get_WorldMatrix());
		return true;
	}

	return false;
}

void CVIBuffer_Rect::Scaling_Texcoord(const _float3& vScale)
{
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

// CGameObject*	p사본 = 원형->Clone();

CComponent * 



CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();


}
