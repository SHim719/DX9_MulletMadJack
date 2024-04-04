#include "CUi.h"
#include "GameInstance.h"

CUi::CUi(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

CUi::CUi(const CUi& rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
	, m_pGameInstance(rhs.m_pGameInstance)
{
	memcpy(&m_UiDesc, &rhs.m_UiDesc, sizeof(Ui_Pos_Size_Rotation));
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CUi::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUi::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CUi::PriorityTick(_float fTimeDelta)
{
}

void CUi::Tick(_float fTimeDelta)
{
}

void CUi::LateTick(_float fTimeDelta)
{
}

HRESULT CUi::Render()
{
	return E_NOTIMPL;
}

HRESULT CUi::Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent** ppOut, void* pArg)
{
	CComponent* pComponent = m_pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	*ppOut = pComponent;

	return S_OK;
}

HRESULT CUi::Add_Components(void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CUi::Add_Texture(void* pArg)
{
	return E_NOTIMPL;
}

void CUi::Default_Set_LifeTime()
{
}

void CUi::Default_Set_Size()
{
}

void CUi::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pGraphic_Device);
}

CUi* CUi::Start(void* pArg)
{
	return nullptr;
}

CUi* CUi::Clone(void* pArg)
{
	return nullptr;
}
