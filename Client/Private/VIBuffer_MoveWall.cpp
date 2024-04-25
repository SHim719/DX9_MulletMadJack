#include "VIBuffer_MoveWall.h"
#include "Transform.h"

CVIBuffer_MoveWall::CVIBuffer_MoveWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_MoveWall::CVIBuffer_MoveWall(const CVIBuffer_MoveWall& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_MoveWall::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; 
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

HRESULT CVIBuffer_MoveWall::Initialize(void* pArg)
{
	m_vTexcoords[0] = _float2(0.0f, 0.f);
	m_vTexcoords[1] = _float2(1.0f, 0.f);
	m_vTexcoords[2] = _float2(1.0f, 1.f);
	m_vTexcoords[3] = _float2(0.0f, 1.f);

	return S_OK;
}

void CVIBuffer_MoveWall::Tick(_float fTimeDelta)
{
	m_vTexcoords[0].y -= m_fMoveSpeed * fTimeDelta;
	m_vTexcoords[1].y -= m_fMoveSpeed * fTimeDelta;
	m_vTexcoords[2].y -= m_fMoveSpeed * fTimeDelta;
	m_vTexcoords[3].y -= m_fMoveSpeed * fTimeDelta;

	VTXNORMAL* pVertices = { nullptr };

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0].vTexcoord = m_vTexcoords[0];
	pVertices[1].vTexcoord = m_vTexcoords[1];
	pVertices[2].vTexcoord = m_vTexcoords[2];
	pVertices[3].vTexcoord = m_vTexcoords[3];
	m_pVB->Unlock();
}


CVIBuffer_MoveWall* CVIBuffer_MoveWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_MoveWall* pInstance = new CVIBuffer_MoveWall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_MoveWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_MoveWall::Clone(void* pArg)
{
	CVIBuffer_MoveWall* pInstance = new CVIBuffer_MoveWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_MoveWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_MoveWall::Free()
{
	__super::Free();


}
