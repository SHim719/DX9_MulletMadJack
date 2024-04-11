#include "SodaMachine_Banner.h"

#include "GameInstance.h"

CSodaMachine_Banner::CSodaMachine_Banner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSodaMachine_Banner::CSodaMachine_Banner(const CSodaMachine_Banner& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSodaMachine_Banner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSodaMachine_Banner::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_iTexture_Index = 1;

	return S_OK;
}

void CSodaMachine_Banner::PriorityTick(_float fTimeDelta)
{
}

void CSodaMachine_Banner::Tick(_float fTimeDelta)
{
}

void CSodaMachine_Banner::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSodaMachine_Banner::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CSodaMachine_Banner::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Soda_Textures"), TEXT("Soda_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}


CSodaMachine_Banner* CSodaMachine_Banner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSodaMachine_Banner* pInstance = new CSodaMachine_Banner(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSodaMachine_Banner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSodaMachine_Banner::Clone(void* pArg)
{
	CSodaMachine_Banner* pInstance = new CSodaMachine_Banner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSodaMachine_Banner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSodaMachine_Banner::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTextureCom);
}