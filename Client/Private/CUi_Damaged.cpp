#include "CUi_Damaged.h"

CUi_Damaged::CUi_Damaged(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Damaged::CUi_Damaged(const CUi_Damaged& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Damaged::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Damaged::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Damaged::PriorityTick(_float fTimeDelta)
{
}

void CUi_Damaged::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	m_fTextureSwitching += fTimeDelta;
	if (m_fActiveTime > 0)
	{
		Adjust_Alpha(fTimeDelta);
	}
	else
	{
		m_bActive = false;
	}

	if (m_fTextureSwitching > 0.1)
	{
		++m_iTexture_IndexId;
		if (m_iTexture_IndexId > 5)
		{
			m_iTexture_IndexId = 0;
		}
	}
}

void CUi_Damaged::LateTick(_float fTimeDelta)
{
}

void CUi_Damaged::Begin_RenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, m_iAlpha));
}

HRESULT CUi_Damaged::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	Begin_RenderState();

	m_pVIBufferCom->Render();

	m_pUniqueTextureCom->Bind_Texture(m_iTexture_IndexId);

	m_pVIBufferCom->Render();
	End_RenderState();

	return S_OK;
}

void CUi_Damaged::End_RenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}

HRESULT CUi_Damaged::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);
	Initialize_Set_Alpha();
	return S_OK;
}

void CUi_Damaged::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.5f;
}

void CUi_Damaged::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = g_iWinSizeX;
	m_UiDesc.m_fSizeY = g_iWinSizeY;
}

void CUi_Damaged::Initialize_Set_Speed()
{
}

void CUi_Damaged::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = 0;

	m_pTransformCom->Set_Scale(Scale);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Damaged::Initialize_Set_Alpha()
{
	m_iAlpha = 100;
}

HRESULT CUi_Damaged::Add_Components(void* pArg)
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

	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Damaged::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Damaged_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Blood_Texture"),
		(CComponent**)&m_pUniqueTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Damaged::Adjust_Alpha(_float fTimeDelta)
{
	m_fAdjustAlphaTime += fTimeDelta;
	if (m_fAdjustAlphaTime > 0.01)
	{
		if (m_iAlpha <= 5)
		{
			m_iAlpha = 0;
		}
		else
		{
			m_iAlpha = m_iAlpha - 3;

		}
		m_fAdjustAlphaTime = 0;
	}
}

CUi_Damaged* CUi_Damaged::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Damaged* pInstance = new CUi_Damaged(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Damaged Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Damaged::Free()
{
	__super::Free();
	Safe_Release(m_pUniqueTextureCom);
}
