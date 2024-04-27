#include "CUi_Sans_Black.h"

CUi_Sans_Black::CUi_Sans_Black(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Sans_Black::CUi_Sans_Black(const CUi_Sans_Black& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Sans_Black::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Sans_Black::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Sans_Black::PriorityTick(_float fTimeDelta)
{
}

void CUi_Sans_Black::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	if (m_fActiveTime <= 0)
	{
		// 소리 다시 켜기
		//m_pGameInstance->Play();
		m_bActive = false;
	}
	//모든 소리 멈추기
	//m_pGameInstance->Stop();
}

void CUi_Sans_Black::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Sans_Black::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Sans_Black::Initialize_Active()
{
	Initialize_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	m_fActiveTime = 0.5f;

	return S_OK;
}

void CUi_Sans_Black::Initialize_Set_Speed()
{
}

void CUi_Sans_Black::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = g_iWinSizeX;
	m_UiDesc.m_fSizeY = g_iWinSizeY;
}

void CUi_Sans_Black::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = 0;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Sans_Black::Add_Components(void* pArg)
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

	Add_Texture(pArg);

	return S_OK;
}

HRESULT CUi_Sans_Black::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Sans_Black_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CUi_Sans_Black* CUi_Sans_Black::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Sans_Black* pInstance = new CUi_Sans_Black(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Sans_Black Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Sans_Black::Free()
{
	__super::Free();
}
