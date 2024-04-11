#include "CUi_Chat.h"
#include "Ui_Pos.h"


CUi_Chat::CUi_Chat(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Chat::CUi_Chat(const CUi_Chat& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Chat::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Chat::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Chat::PriorityTick(_float fTimeDelta)
{
}

void CUi_Chat::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	m_fTextureTime += fTimeDelta;
	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Chat);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}

	if (m_fTextureTime > 0.3)
	{
		Texture_Switching();
		m_fTextureTime = 0.f;
	}
}

void CUi_Chat::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Chat::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUi_Chat::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);


	return S_OK;
}

void CUi_Chat::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Chat::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 200;
	m_UiDesc.m_fSizeY = 250;
}

void CUi_Chat::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1000);
}

void CUi_Chat::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 730.f;
	m_UiDesc.m_fY = -500.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Chat::Add_Components(void* pArg)
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

HRESULT CUi_Chat::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Chat_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CUi_Chat::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Active();
	}
	else
	{
		Initialize_Set_ActiveTime();
	}
}

void CUi_Chat::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	else
	{
		m_pTransformCom->Set_Speed(1500);
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CUi_Chat::Texture_Switching()
{
	m_iTexture_Index = rand() % m_pTextureCom->Get_MaxTextureNum();
}

CUi_Chat* CUi_Chat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Chat* pInstance = new CUi_Chat(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_PEACE Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Chat::Free()
{
	__super::Free();
}
