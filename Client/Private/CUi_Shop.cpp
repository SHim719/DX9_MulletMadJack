#include "CUi_Shop.h"
#include "CGame_Manager.h"
#include "GameInstance.h"


CUi_Shop::CUi_Shop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Shop::CUi_Shop(const CUi_Shop& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Shop::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Shop::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Shop::PriorityTick(_float fTimeDelta)
{
}

void CUi_Shop::Tick(_float fTimeDelta)
{
	m_fMoveTime -= fTimeDelta;
	Move(fTimeDelta);
	Player_Shopping();
}

void CUi_Shop::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Shop::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Shop::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return S_OK;
}

void CUi_Shop::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0;
}

void CUi_Shop::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 1150;
	m_UiDesc.m_fSizeY = 600;
}

void CUi_Shop::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1200);
}

void CUi_Shop::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 0.f;
	m_UiDesc.m_fY = -770.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Shop::Add_Components(void* pArg)
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

HRESULT CUi_Shop::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Shop_Test_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Shop::Move(_float fTimeDelta)
{
	if (!m_bEnd)
	{
		if (m_fMoveTime > 0.5)
		{
			m_pTransformCom->Go_Up(fTimeDelta);
		}
		else if (m_fMoveTime < 0.5 && m_fMoveTime >0)
		{
			Scaling(fTimeDelta);
			m_pTransformCom->Set_Speed(100);
			m_pTransformCom->Go_Down(fTimeDelta);
		}
	}
	else
	{
		if (m_fMoveTime > 0.8)
		{
			m_pTransformCom->Set_Speed(300);
			m_pTransformCom->Go_Up(fTimeDelta);
		}
		else if (m_fMoveTime > 0)
		{
			m_pTransformCom->Set_Speed(1200);
			m_pTransformCom->Go_Down(fTimeDelta);
		}
		else
		{
			m_bEnd = false;
			m_bActive = false;
		}
	}
}

void CUi_Shop::Scaling(_float fTimeDelta)
{
	m_UiDesc.m_fSizeX += 3500 * fTimeDelta;
	m_UiDesc.m_fSizeY += 1200 * fTimeDelta;
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1 };
	m_pTransformCom->Set_Scale(Scale);

}

void CUi_Shop::Player_Shopping()
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT			UI;
	//SetRect(&UI, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);
	SetRect(&UI, 500, 300, 700, 400);
	if (true == (bool)PtInRect(&UI, ptMouse) && !m_bEnd)
	{
		if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
			Player_Choice();
	}
}

void CUi_Shop::Player_Choice()
{
	CGame_Manager::Get_Instance()->Player_UpGrade(nullptr);
	m_bEnd = true;
	m_fMoveTime = 1.f;
	CGame_Manager::Get_Instance()->
		Set_StageProgress(CGame_Manager::StageProgress::Changing);
}

CUi_Shop* CUi_Shop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Shop* pInstance = new CUi_Shop(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Shop Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Shop::Free()
{
	__super::Free();
}
