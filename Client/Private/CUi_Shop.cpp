#include "CUi_Shop.h"
#include "CGame_Manager.h"
#include "GameInstance.h"
#include "CUi_Shop_UpGrade.h"


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

	m_UpgradeVec.reserve(3);

	m_PickScale.reserve(3);
	m_PickScale.push_back(RECT{ 270, 230, 440, 550 });
	m_PickScale.push_back(RECT{ 460, 230, 630, 550 });
	m_PickScale.push_back(RECT{ 650, 230, 820, 550 });

	return S_OK;
}

HRESULT CUi_Shop::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Shop::PriorityTick(_float fTimeDelta)
{
	m_fChangeProgressTime -= fTimeDelta;
	Change_Progress(fTimeDelta);
	for (auto& iter : m_UpgradeVec)
		iter->PriorityTick(fTimeDelta);
}

void CUi_Shop::Tick(_float fTimeDelta)
{
	for (auto& iter : m_UpgradeVec)
		iter->Tick(fTimeDelta);

	Move(fTimeDelta);

	if (m_eProgress == ShopProgress::Shopping
		|| m_eProgress == ShopProgress::Select
		|| m_eProgress == ShopProgress::End)
	{
		Set_UpgradePos();
	}

	Texture_Switching(fTimeDelta);
}

void CUi_Shop::LateTick(_float fTimeDelta)
{
	if (m_eProgress == ShopProgress::Shopping)
	{
		Check_Picking();
	}

	for (auto& iter : m_UpgradeVec)
		iter->LateTick(fTimeDelta);
}

HRESULT CUi_Shop::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	if (m_eProgress == ShopProgress::Shopping
		|| m_eProgress == ShopProgress::Select
		|| m_eProgress == ShopProgress::End)
	{
		for (auto& iter : m_UpgradeVec)
		{
			iter->Render();
		}
	}

	return S_OK;
}

HRESULT CUi_Shop::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Progress_TextureIndex();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	Initialize_UpGradeVec();

	return S_OK;
}

void CUi_Shop::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0;
	m_fMoveTime = 1.f;
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

void CUi_Shop::Initialize_Progress_TextureIndex()
{
	m_eProgress = ShopProgress::Clear;
	m_fChangeProgressTime = 2.5;
	m_iTexture_Index = 1;
}

void CUi_Shop::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 0.f;
	m_UiDesc.m_fY = -670.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Shop::Initialize_UpGradeVec()
{
	for (auto iter : m_UpgradeVec)
		Safe_Release(iter);

	int Levelid = m_pGameInstance->Get_CurrentLevelID() - (_uint)LEVEL::LEVEL_GAMEPLAY;
	m_UpgradeVec.clear();
	CUi_Shop_UpGrade* part[3];
	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	for (size_t i = 0; i < m_UpgradeVec.capacity(); ++i)
	{
		part[i] = (CUi_Shop_UpGrade*)m_pGameInstance->
			Add_Ui_PartClone(TEXT("CUi_Shop_UpGrade"), &Pos);
		part[i]->Set_Texture_Index(_uint(Levelid*3 + i));
		part[i]->Set_UniqueTextureIndex((_uint)i);
		m_UpgradeVec.emplace_back(part[i]);
	}
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
		TEXT("CUi_Shop_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Shop::Change_Progress(_float fTimeDelta)
{
	if (m_eProgress == ShopProgress::Clear && m_fChangeProgressTime < 0)
	{
		m_eProgress = ShopProgress::ApproachUp;
		m_fChangeProgressTime = 1.f;
		m_fMoveTime = 1.f;
	}
	else if (m_eProgress == ShopProgress::Approach && m_fChangeProgressTime < 0)
	{
		m_eProgress = ShopProgress::Noise;
		m_pTransformCom->Set_Scale(m_OriginScale);
		m_pTransformCom->Set_Position(m_OriginPos);
		m_fChangeProgressTime = 0.5;
	}
	else if (m_eProgress == ShopProgress::Noise && m_fChangeProgressTime < 0)
	{
		m_eProgress = ShopProgress::Shopping;
	}
	else if (m_eProgress == ShopProgress::Select && m_fChangeProgressTime < 0.f)
	{
		m_fMoveTime = 1.f;
		m_fChangeProgressTime = 1.f;
		m_eProgress = ShopProgress::End;
	}
	else if (m_eProgress == ShopProgress::End && m_fChangeProgressTime <= 0)
	{
		CGame_Manager::Get_Instance()->
			Set_StageProgress(CGame_Manager::StageProgress::Changing);
		m_bActive = false;
	}
}

void CUi_Shop::Move(_float fTimeDelta)
{
	m_fMoveTime -= fTimeDelta;
	if (m_eProgress == ShopProgress::Approach
		|| m_eProgress == ShopProgress::ApproachUp)
	{
		Approach(fTimeDelta);
	}
	else if(m_eProgress == ShopProgress::End)
	{
		Move_Down(fTimeDelta);
	}
}

void CUi_Shop::Scaling(_float fTimeDelta)
{
	m_UiDesc.m_fSizeX += 3500 * fTimeDelta;
	m_UiDesc.m_fSizeY += 1200 * fTimeDelta;
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1 };
	m_pTransformCom->Set_Scale(Scale);

}

void CUi_Shop::Approach(_float fTimeDelta)
{
	if (m_fMoveTime > 0.5 && m_eProgress == ShopProgress::ApproachUp)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
	else if (m_fMoveTime < 0.5 && m_fMoveTime >0)
	{
		m_eProgress = ShopProgress::Approach;
		Scaling(fTimeDelta);
		m_pTransformCom->Set_Speed(100);
		m_pTransformCom->Go_Down(fTimeDelta);
	}
}

void CUi_Shop::Move_Down(_float fTimeDelta)
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
}

void CUi_Shop::Texture_Switching(_float fTimeDelta)
{
	m_fTextureSwitchingTime += fTimeDelta;
	m_fUniqueTextureSwitchingTime += fTimeDelta;
	if (m_fTextureSwitchingTime>0.1f && m_eProgress == ShopProgress::Approach)
	{
		m_fTextureSwitchingTime = 0;
		++m_iTexture_Index;
		if (m_iTexture_Index >= 5)
		{
			m_iTexture_Index = 5;
		}		
	}
	else if (m_fTextureSwitchingTime > 0.07f && m_eProgress == ShopProgress::Noise)
	{
		++m_iTexture_Index;
		if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
		{
			m_iTexture_Index = 6;
		}
	}

	if(m_eProgress == ShopProgress::Shopping || m_eProgress == ShopProgress::End)
		m_iTexture_Index = 0;

	if (m_fUniqueTextureSwitchingTime > 0.3)
	{
		m_fUniqueTextureSwitchingTime = 0;
		for (auto& iter : m_UpgradeVec)
			iter->Add_UniqueTextureIndex();
	}
}

void CUi_Shop::Set_UpgradePos()
{
	_float3 Pos;  
	for (size_t i = 0; i < m_UpgradeVec.size(); ++i)
	{
		Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		Pos.x += -160;
		Pos.y += +20;
		Pos.x += i * 255;
		m_UpgradeVec[i]->Set_Pos(Pos);	
	}
}

void CUi_Shop::Check_Picking()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for (_uint i = 0; i < (_uint)m_PickScale.size(); ++i)
	{
		if (true == (bool)PtInRect(&m_PickScale[i], ptMouse))
		{
			if (!Player_Select(i))
				m_UpgradeVec[i]->Set_Focusing(true);
			else
				m_UpgradeVec[i]->Set_Focusing(false);
		}
		else
		{
			m_UpgradeVec[i]->Set_Focusing(false);
		}
	}
}

bool CUi_Shop::Player_Select(_uint iNumber)
{
	if ((m_pGameInstance->GetKeyDown(eKeyCode::LButton)))
	{
		m_UpgradeVec[iNumber]->Set_Picked();
		m_eProgress = ShopProgress::Select;
		m_fChangeProgressTime = 1;
		return true;
	}
	return false;
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
	for (auto& iter : m_UpgradeVec)
		Safe_Release(iter);

	m_UpgradeVec.clear(); 

	__super::Free();
}
