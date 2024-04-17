#include "CUi_Announcer.h"
#include "Ui_Pos.h"
#include "CGame_Manager.h"
#include "GameInstance.h"
#include "CText_BackGround.h"


CUi_Announcer::CUi_Announcer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
	,m_pGameManager(CGame_Manager::Get_Instance())
{
}

CUi_Announcer::CUi_Announcer(const CUi_Announcer& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Announcer::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Announcer::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Announcer::PriorityTick(_float fTimeDelta)
{
}

void CUi_Announcer::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	m_fUniqueTextureTime += fTimeDelta;
	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Announcer);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}

	if (m_fUniqueTextureTime > 0.2)
	{
		Texture_Switching(fTimeDelta);
		m_fUniqueTextureTime = 0;
	}

	Set_Pos_TextBackGround();
}

void CUi_Announcer::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Announcer::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	m_pUniqueTexture->Bind_Texture(m_iUniqueTextureIndex);
	m_pVIBufferCom->Render();
	
	TextRender();

	return S_OK;
}

HRESULT CUi_Announcer::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);
	Ui_Pos_Size_Rotation TextBack = m_UiDesc;
	TextBack.m_fX = m_UiDesc.m_fX - 800;
	TextBack.m_fSizeX = 650;
	TextBack.m_fSizeY = 100;
	if (m_pTextBackGround == nullptr)
	{
		m_pTextBackGround = (CText_BackGround*)m_pGameInstance->
			Add_Ui_PartClone(L"CText_BackGround", &TextBack);
	}

	return S_OK;
}

void CUi_Announcer::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Announcer::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 270;
	m_UiDesc.m_fSizeY = 310;
}

void CUi_Announcer::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(2300);
}

void CUi_Announcer::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 910.f;
	m_UiDesc.m_fY = 200.f;

	m_pTransformCom->Set_Scale(Scale);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Announcer::Add_Components(void* pArg)
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

HRESULT CUi_Announcer::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Announcer_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Sheet_Texture"),
		(CComponent**)&m_pUniqueTexture)))
		return E_FAIL;


	return S_OK;
}

void CUi_Announcer::Enter(bool _Enter)
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

void CUi_Announcer::Move(_float fTimeDelta)
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

void CUi_Announcer::Texture_Switching(_float fTimeDelta)
{
	++m_iUniqueTextureIndex;
	if (m_iUniqueTextureIndex > m_pUniqueTexture->Get_MaxTextureNum())
		m_iUniqueTextureIndex = 0;
}

void CUi_Announcer::TextRender()
{
	if (m_pGameManager->Get_StageProgress() == StageProgress::ShopEnd
		|| m_pGameManager->Get_StageProgress() == StageProgress::Changing)
	{
		if (m_pGameManager->Get_MaxSize(TextType::ShopEnd) >
			m_pGameManager->Get_TextNumber(TextType::ShopEnd))
		{
			m_pTextBackGround->Render();
			m_pGameManager->Print_Text(TextType::ShopEnd,
				0);
		}
	}
	else if(m_pGameManager->Get_StageProgress() == 
		StageProgress::TutorialMidSpot)
	{
		if (m_pGameManager->Get_MaxSize(TextType::TutorialMidSpot) >
			m_pGameManager->Get_TextNumber(TextType::TutorialMidSpot))
		{
			m_pTextBackGround->Render();
			m_pGameManager->Print_Text(TextType::TutorialMidSpot,
				m_pGameManager->Get_TextNumber(TextType::TutorialMidSpot));
		}
	}
	else if (m_pGameManager->Get_StageProgress() ==
		StageProgress::TutorialClear)
	{
		if (m_pGameManager->Get_MaxSize(TextType::TutorialClear) >
			m_pGameManager->Get_TextNumber(TextType::TutorialClear))
		{
			m_pTextBackGround->Render();
			m_pGameManager->Print_Text(TextType::TutorialClear,
				m_pGameManager->Get_TextNumber(TextType::TutorialClear));
		}
	}
}

void CUi_Announcer::Set_Pos_TextBackGround()
{
	_float3 BackPos = m_pTransformCom->Get_Pos();
	BackPos.x -= 500;
	BackPos.y += 100;
	m_pTextBackGround->Set_Pos(BackPos);
}

CUi_Announcer* CUi_Announcer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Announcer* pInstance = new CUi_Announcer(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Announcer Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Announcer::Free()
{
	Safe_Release(m_pTextBackGround);
	Safe_Release(m_pUniqueTexture);
	__super::Free();
}
