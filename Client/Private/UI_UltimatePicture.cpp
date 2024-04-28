#include "UI_UltimatePicture.h"
#include "GameInstance.h"

CUI_UltimatePicture::CUI_UltimatePicture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUI_UltimatePicture::CUI_UltimatePicture(const CUI_UltimatePicture& rhs)
	: CUi(rhs)
{
}

HRESULT CUI_UltimatePicture::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CUI_UltimatePicture::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	m_vecTargetPosition.reserve(3);
	m_vecTargetPosition.push_back({ 150.f, 0.f, 0.f });
	m_vecTargetPosition.push_back({ 0.f, 0.f, 0.f });
	m_vecTargetPosition.push_back({ -800.f, 0.f, 0.f });

	m_vecTargetTime.reserve(3);
	m_vecTargetTime.push_back(0.2f);
	m_vecTargetTime.push_back(1.f);
	m_vecTargetTime.push_back(0.2f);

	return S_OK;
}

HRESULT CUI_UltimatePicture::Initialize_Active()
{
	m_iNowIdx = 0;
	m_iTexture_Index = 0;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CUI_UltimatePicture::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CUI_UltimatePicture::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= m_vecTargetTime[m_iNowIdx])
	{
		m_fTimeAcc = 0.f;
		m_UiDesc.m_fX = m_vecTargetPosition[m_iNowIdx].x;
		m_iNowIdx++;
		if (m_iNowIdx == m_vecTargetPosition.size())
		{
			m_bActive = false;
			return;
		}
	}
		
	m_vPosition.x = m_UiDesc.m_fX + (m_vecTargetPosition[m_iNowIdx].x - m_UiDesc.m_fX) * m_fTimeAcc /  m_vecTargetTime[m_iNowIdx];

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Set_Pos(m_vPosition);
}

void CUI_UltimatePicture::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_UltimatePicture::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_UltimatePicture::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(640, 0);

	m_fScale = { m_UiDesc.m_fSizeX , m_UiDesc.m_fSizeY , 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUI_UltimatePicture::Initialize_Set_Speed()
{
}

void CUI_UltimatePicture::Default_Set_LifeTime()
{
}

void CUI_UltimatePicture::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 600;
	m_UiDesc.m_fSizeY = 120;
}

HRESULT CUI_UltimatePicture::Add_Components(void* pArg)
{
	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;

	return S_OK;
}


HRESULT CUI_UltimatePicture::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("UI_Ultimate_Picture_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CUI_UltimatePicture::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_UltimatePicture* pInstance = new CUI_UltimatePicture(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_UltimatePicture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CUI_UltimatePicture::Free()
{
	__super::Free();
}
