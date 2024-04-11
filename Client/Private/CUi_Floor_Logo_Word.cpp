#include "CUi_Floor_Logo_Word.h"
#include "GameInstance.h"
#include "CUi_Floor_Logo_Num.h"


CUi_Floor_Logo_Word::CUi_Floor_Logo_Word(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Floor_Logo_Word::CUi_Floor_Logo_Word(const CUi_Floor_Logo_Word& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Floor_Logo_Word::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Floor_Logo_Word::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Floor_Logo_Word::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor_Logo_Word::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}
}

void CUi_Floor_Logo_Word::LateTick(_float fTimeDelta)
{

	for (size_t i = 0; i < m_pLogoNum.size(); ++i)
	{
		_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		Pos.x += (i * 40) + 90;
		m_pLogoNum[i]->Set_Pos(Pos);
	}

}

HRESULT CUi_Floor_Logo_Word::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	for (auto& iter : m_pLogoNum)
		iter->Render();

	return S_OK;
}

HRESULT CUi_Floor_Logo_Word::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);
	Initialize_LogoNumVec();

	return S_OK;
}

void CUi_Floor_Logo_Word::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Floor_Logo_Word::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 200;
	m_UiDesc.m_fSizeY = 40;
}

void CUi_Floor_Logo_Word::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1000);
}

void CUi_Floor_Logo_Word::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 760.f;
	m_UiDesc.m_fY = -280.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Floor_Logo_Word::Initialize_LogoNumVec()
{
	for (auto iter : m_pLogoNum)
		Safe_Release(iter);
	m_pLogoNum.clear();

	m_pLogoNum.reserve(2);

	CUi_Floor_Logo_Num* First = (CUi_Floor_Logo_Num*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_Floor_Logo_Num"), &m_UiDesc);
	m_pLogoNum.push_back(First);

	CUi_Floor_Logo_Num* Second = (CUi_Floor_Logo_Num*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_Floor_Logo_Num"), &m_UiDesc);
	Second->Set_LevelTexture();

	m_pLogoNum.push_back(Second);
}

HRESULT CUi_Floor_Logo_Word::Add_Components(void* pArg)
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

HRESULT CUi_Floor_Logo_Word::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Floor_Logo_Word_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Floor_Logo_Word::Enter(bool _Enter)
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

void CUi_Floor_Logo_Word::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Speed(1300);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

CUi_Floor_Logo_Word* CUi_Floor_Logo_Word::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Floor_Logo_Word* pInstance = new CUi_Floor_Logo_Word(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Floor_Logo_Word Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Floor_Logo_Word::Free()
{
	for (auto iter : m_pLogoNum)
		Safe_Release(iter);

	m_pLogoNum.clear();

	__super::Free();
}
