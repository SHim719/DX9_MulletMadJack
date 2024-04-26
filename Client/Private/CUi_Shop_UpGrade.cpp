#include "CUi_Shop_UpGrade.h"
#include "GameInstance.h"
#include "CUi_UpGrade_Select.h"
#include "CUi_UpGrade_Focus.h"
#include "PlayerManager.h"

CUi_Shop_UpGrade::CUi_Shop_UpGrade(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Shop_UpGrade::CUi_Shop_UpGrade(const CUi_Shop_UpGrade& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Shop_UpGrade::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_Shop_UpGrade::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();

	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pSelect = (CUi_UpGrade_Select*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_UpGrade_Select"), &Pos);
	m_pFocus = (CUi_UpGrade_Focus*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_UpGrade_Focus"), &Pos);

	return S_OK;
}

void CUi_Shop_UpGrade::PriorityTick(_float fTimeDelta)
{
}

void CUi_Shop_UpGrade::Tick(_float fTimeDelta)
{
	m_pFocus->Tick(fTimeDelta);
	m_pSelect->Tick(fTimeDelta);
	if (m_bFocused)
	{
		m_pFocus->Set_Pos(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	if (m_bPicked)
	{
		m_pSelect->Set_Pos(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
}

void CUi_Shop_UpGrade::LateTick(_float fTimeDelta)
{
	if (m_bFocused || m_bPicked)
	{
		_float3 ChangePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		ChangePos.y -= 30;
		m_pTransformCom->Set_Position(ChangePos);
	}
}

HRESULT CUi_Shop_UpGrade::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();
	
	
	m_pUniqueTexture->Bind_Texture(m_iUniqueTextureIndex);
	m_pVIBufferCom->Render();

	if (m_bFocused)
	{
		m_pFocus->Render();
	}
	if (m_bPicked)
	{
		m_pSelect->Render();
	}

	return S_OK;
}

HRESULT CUi_Shop_UpGrade::Add_Components(void* pArg)
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

HRESULT CUi_Shop_UpGrade::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Shop_UpGrade_Texture")
		,(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Sheet_Texture"),
		(CComponent**)&m_pUniqueTexture)))
		return E_FAIL;

	return S_OK;
}

void CUi_Shop_UpGrade::Default_Set_LifeTime()
{
	m_fLifeTime = 0.f;
}

void CUi_Shop_UpGrade::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 230;
	m_UiDesc.m_fSizeY = 390;
}

void CUi_Shop_UpGrade::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3* Arg = (_float3*)pArg;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 0.f };
	
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		Arg);
}

void CUi_Shop_UpGrade::Initialize_Set_Speed()
{
}

void CUi_Shop_UpGrade::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

void CUi_Shop_UpGrade::Set_Picked()
{
	m_bPicked = true;
	if (m_iTexture_Index == 0)
	{
		CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer::WEAPON_TYPE::PISTOL);
	}
	else if (m_iTexture_Index == 1)
	{
		CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer::WEAPON_TYPE::SHOTGUN);
	}
	else if (m_iTexture_Index == 2)
	{
		CPlayer_Manager::Get_Instance()->WeaponChange(CPlayer::WEAPON_TYPE::KATANA);
	}
}

void CUi_Shop_UpGrade::Add_UniqueTextureIndex()
{
	++m_iUniqueTextureIndex;
	if (m_iUniqueTextureIndex > m_pUniqueTexture->Get_MaxTextureNum())
	{
		m_iUniqueTextureIndex = 0;
	}
}

void CUi_Shop_UpGrade::Set_UniqueTextureIndex(_uint _IndexNum)
{
	if (_IndexNum > m_pUniqueTexture->Get_MaxTextureNum())
	{
		m_iUniqueTextureIndex = 0;
		return;
	}
	m_iUniqueTextureIndex = _IndexNum;
}

CUi_Shop_UpGrade* CUi_Shop_UpGrade::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Shop_UpGrade* pInstance = new CUi_Shop_UpGrade(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Shop_UpGrade Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Shop_UpGrade::Clone(void* pArg)
{
	CUi_Shop_UpGrade* pInstance = new CUi_Shop_UpGrade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Shop_UpGrade Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Shop_UpGrade::Free()
{
	Safe_Release(m_pUniqueTexture);
	Safe_Release(m_pSelect);
	Safe_Release(m_pFocus);
	__super::Free();
}
