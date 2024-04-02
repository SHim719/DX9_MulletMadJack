#include "..\Public\Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	return S_OK;
}

void CPlayer::PriorityTick(_float fTimeDelta)
{
}

void CPlayer::Tick(_float fTimeDelta)
{
}

void CPlayer::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
	//if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
	//	return E_FAIL;
	//
	//if (FAILED(m_pTextureCom->Bind_Texture(0)))
	//	return E_FAIL;
	//
	//if (FAILED(Begin_RenderState()))
	//	return E_FAIL;
	//
	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;
	//
	//if (FAILED(End_RenderState()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	//m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_TOOL, L"Wall1_Textures"));
	//if (nullptr == m_pTextureCom)
	//	return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
	//	TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;
	return S_OK;
}

HRESULT CPlayer::Begin_RenderState()
{
	HRESULT hr = S_OK;
	hr += m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	hr += m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	hr += m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return hr;
}

HRESULT CPlayer::End_RenderState()
{
	HRESULT hr = m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return hr;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

// CGameObject*	p사본 = 원형->Clone();

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
