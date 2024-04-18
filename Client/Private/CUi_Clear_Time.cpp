#include "CUi_Clear_Time.h"
#include "GameInstance.h"


CUi_Clear_Time::CUi_Clear_Time(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Clear_Time::CUi_Clear_Time(const CUi_Clear_Time& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Clear_Time::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Clear_Time::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Clear_Time::PriorityTick(_float fTimeDelta)
{
}

void CUi_Clear_Time::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	m_fCallClearTime -= fTimeDelta;

	if (m_fCallClearTime < 0 && !m_bCallClearTime)
	{
		m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_Real_ClearTime"),
			eUiRenderType::Render_NonBlend, nullptr);
		m_bCallClearTime = true;
	}
	Scaling(fTimeDelta);

	if (m_fActiveTime < 0)
	{
		m_bCallClearTime = false;
		m_bActive = false;
	}
}

void CUi_Clear_Time::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Clear_Time::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Clear_Time::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	m_fScaleTime = 0.3f;

	return S_OK;
}

void CUi_Clear_Time::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 2.f;
	m_fCallClearTime = 0.5f;
}

void CUi_Clear_Time::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 6000;
	m_UiDesc.m_fSizeY = 650;
}

void CUi_Clear_Time::Initialize_Set_Speed()
{
}

void CUi_Clear_Time::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = -150.f;
	m_UiDesc.m_fY = 150.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Clear_Time::Add_Components(void* pArg)
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

HRESULT CUi_Clear_Time::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Clear_Time_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Clear_Time::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Active();
	}
	else
	{
		m_bActive = false;
	}
}

void CUi_Clear_Time::Scaling(_float fTimeDelta)
{
	m_fScaleTime -= fTimeDelta;
	if (m_fScaleTime > 0)
	{
		_float3 Scale = m_pTransformCom->Get_Scale();
		Scale.x -= fTimeDelta * 19500;
		Scale.y -= fTimeDelta * 2000;
		m_pTransformCom->Set_Scale(Scale);
	}
	else
	{
		m_pTransformCom->Set_Scale(m_OriginScale);
	}
}

CUi_Clear_Time* CUi_Clear_Time::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Clear_Time* pInstance = new CUi_Clear_Time(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Clear_Time Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Clear_Time::Free()
{
	__super::Free();
}
