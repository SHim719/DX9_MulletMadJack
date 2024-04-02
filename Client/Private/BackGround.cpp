#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackGround::PriorityTick(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Tick(_float fTimeDelta)
{
	if(GetKeyState(VK_UP) & 0x8000)
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Go_Right(fTimeDelta);

}

void CBackGround::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBackGround::Render()
{	
	/* 장치에다가 월드, 뷰, 투영행렬을 바인딩하면. */
	/* 이후 로컬스페이스 상에 정의된 정점들을 그린다라고 이야기를 했을때 */
	/* 저장하고있던 월드, 뷰, 투영행렬을 장치가 알아서 정점위치에 곱하여 렌더링한다. */
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	

	/* 여기서 셋팅하는 카메라의  */
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, 
		D3DXMatrixLookAtLH(&m_ViewMatrix, 
			&_float3(0.f, 2.0f, -1.5f), 
			&_float3(0.f, 0.f, 0.f), 
			&_float3(0.f, 1.f, 0.f)));
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, D3DXToRadian(60.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 300.f));

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBackGround::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC	TransformDesc{};

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	m_pTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), &TransformDesc));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*	pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}
