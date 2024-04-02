#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent{ rhs }
	, m_pVB{ rhs.m_pVB }
	, m_iVertexStride{ rhs.m_iVertexStride }
	, m_iNumVertices{ rhs.m_iNumVertices }
	, m_ePrimitiveType { rhs.m_ePrimitiveType }
	, m_FVF{ rhs.m_FVF }
	, m_iNumPrimitives{ rhs.m_iNumPrimitives }
	, m_pIB { rhs.m_pIB }
	, m_iIndexSizeofPrimitive{ rhs.m_iIndexSizeofPrimitive }
	, m_eIndexFormat { rhs.m_eIndexFormat } 
{
	Safe_AddRef(m_pVB);	
	Safe_AddRef(m_pIB);	
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->SetFVF(m_FVF);


	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitives);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 정점 배열을 할당한다. */
	/* 1. 몇바이트나할당할까? 정점하나의 크기(Byte) * 정점의 갯수 */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_FVF, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSizeofPrimitive * m_iNumPrimitives, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
