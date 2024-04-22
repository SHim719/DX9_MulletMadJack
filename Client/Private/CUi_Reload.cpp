#include "CUi_Reload.h"

CUi_Reload::CUi_Reload(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Reload::CUi_Reload(const CUi_Reload& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Reload::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Reload::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Reload::PriorityTick(_float fTimeDelta)
{
}

void CUi_Reload::Tick(_float fTimeDelta)
{
	Blink(fTimeDelta);
}

void CUi_Reload::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Reload::Render()
{
	if (m_bBlink)
	{
		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		m_pTextureCom->Bind_Texture(m_iTexture_Index);
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

HRESULT CUi_Reload::Initialize_Active()
{
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return S_OK;
}

void CUi_Reload::Initialize_Set_Speed()
{
	m_UiDesc.m_fSizeX = 200;
	m_UiDesc.m_fSizeY = 50;
}

void CUi_Reload::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = -50.f;
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Reload::Add_Components(void* pArg)
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

HRESULT CUi_Reload::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Reload_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Reload::Blink(_float fTimeDelta)
{
	if (m_bBlink)
	{
		m_fBlink += fTimeDelta;
		if (m_fBlink > 0.5f)
		{
			m_fBlink = 0.f;
			m_bBlink = false;
		}	
	}
	else
	{
		m_fBlinkGap += fTimeDelta;
		if (m_fBlinkGap > 0.5f)
		{
			m_bBlink = true;
			m_fBlinkGap = 0;
		}
	}
}

CUi_Reload* CUi_Reload::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Reload* pInstance = new CUi_Reload(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Reload Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Reload::Free()
{
	__super::Free();
}
