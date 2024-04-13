#include "CUi_Border.h"
#include "Ui_Pos.h"


CUi_Border::CUi_Border(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Border::CUi_Border(const CUi_Border& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Border::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Border::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Border::PriorityTick(_float fTimeDelta)
{
}

void CUi_Border::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
		Scaling(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		m_pTransformCom->Set_Scale(m_OriginScale);
		m_pUniqueTransformCom->Set_Scale(m_OriginUniqueScale);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Border_Right);
		m_pUniqueTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Border_Left);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}
}

void CUi_Border::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Border::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	if (FAILED(m_pUniqueTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pUniqueVIBufferCom->Render();


	return S_OK;
}

HRESULT CUi_Border::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);


	return S_OK;
}

void CUi_Border::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Border::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 280;
	m_UiDesc.m_fSizeY = 320;
	m_UniqueUiDesc.m_fSizeX = g_iWinSizeX-20;
	m_UniqueUiDesc.m_fSizeY = g_iWinSizeY-20;
}

void CUi_Border::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(2300);
	m_pUniqueTransformCom->Set_Speed(400);
}

void CUi_Border::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	_float3 UniqueScale = { m_UniqueUiDesc.m_fSizeX, m_UniqueUiDesc.m_fSizeY, 1.f };
	m_UiDesc.m_fX = 910.f;
	m_UiDesc.m_fY = 200.f;
	m_UniqueUiDesc.m_fX = 0;
	m_UniqueUiDesc.m_fY = 0;

	m_pTransformCom->Set_Scale(Scale);
	m_pUniqueTransformCom->Set_Scale(UniqueScale);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pUniqueTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UniqueUiDesc.m_fX, m_UniqueUiDesc.m_fY, 0));
}

HRESULT CUi_Border::Add_Components(void* pArg)
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


	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pUniqueVIBufferCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pUniqueTransformCom)))
		return E_FAIL;


	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;


	return S_OK;

}

HRESULT CUi_Border::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Border_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}
void CUi_Border::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Active();
	}
	else
	{
		Initialize_Set_Speed();
		Initialize_Set_ActiveTime();
	}
}

void CUi_Border::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		if (m_fActiveTime < 0.2f)
		{
			m_pTransformCom->Set_Speed(1200);
			m_pTransformCom->Go_Left(fTimeDelta);
		}
		else
			m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		if (m_fActiveTime < 0.15f)
		{
			m_pTransformCom->Go_Right(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Set_Speed(1600);
			m_pTransformCom->Go_Right(fTimeDelta);
		}
	}
}

void CUi_Border::Scaling(_float fTimeDelta)
{
	if (m_UniqueUiDesc.m_fSizeX > g_iWinSizeX - 300 && m_bEnter)
	{
		m_UniqueUiDesc.m_fSizeX -= DWORD(fTimeDelta * 1500);
		m_UniqueUiDesc.m_fSizeY -=  DWORD(fTimeDelta * 600);
	}
	else if (!m_bEnter)
	{
		m_UniqueUiDesc.m_fSizeX += DWORD(fTimeDelta * 1500);
		m_UniqueUiDesc.m_fSizeY += DWORD(fTimeDelta * 600);
	}
	_float3 UniqueScale = { m_UniqueUiDesc.m_fSizeX, m_UniqueUiDesc.m_fSizeY, 1.f };
	m_pUniqueTransformCom->Set_Scale(UniqueScale);
	Scaling_Move(fTimeDelta);
}

void CUi_Border::Scaling_Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pUniqueTransformCom->Set_Speed(240);
		m_pUniqueTransformCom->Go_Up(fTimeDelta);

		m_pUniqueTransformCom->Set_Speed(550);
		m_pUniqueTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pUniqueTransformCom->Set_Speed(300);
		m_pUniqueTransformCom->Go_Down(fTimeDelta);

		m_pUniqueTransformCom->Set_Speed(730);
		m_pUniqueTransformCom->Go_Right(fTimeDelta);
	}
}

CUi_Border* CUi_Border::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Border* pInstance = new CUi_Border(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Border Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Border::Free()
{
	Safe_Release(m_pUniqueTransformCom);
	Safe_Release(m_pUniqueVIBufferCom);
	__super::Free();
}
