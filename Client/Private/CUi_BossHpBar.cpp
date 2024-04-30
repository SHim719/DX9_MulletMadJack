#include "CUi_BossHpBar.h"
#include "Beholder.h"
#include "Artemis.h"
#include "CGame_Manager.h"
#include "Apollo.h"


CUi_BossHpBar::CUi_BossHpBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_BossHpBar::CUi_BossHpBar(const CUi_BossHpBar& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_BossHpBar::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_BossHpBar::Initialize(void* pArg)
{

	return S_OK;
}

void CUi_BossHpBar::PriorityTick(_float fTimeDelta)
{
}

void CUi_BossHpBar::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Pos(m_OriginPos);
	m_pBackGroundTransformCom->Set_Pos(m_OriginPos);
}

void CUi_BossHpBar::LateTick(_float fTimeDelta)
{
	Check_BossDead();

	Decision_Type();
	Set_HpTextureIndex();
	LockHpBar();
	Shaking(fTimeDelta);
}

void CUi_BossHpBar::Render_Begin()
{
	switch (m_eFocusType)
	{
	case Client::BossType::Beholder:
		Render_BeginBeholder();
		break;
	case Client::BossType::Artemis:
		Render_BeginArtemis();
		break;
	case Client::BossType::Apollon:
		Render_BeginApollon();
		break;
	default:
		assert(false);
		break;
	}

}

void CUi_BossHpBar::Render_BeginBeholder()
{
	if (m_bLockHpBar)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(90, 90, 90, 255));
	}
	else
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

void CUi_BossHpBar::Render_BeginArtemis()
{
	if (!m_bBigShaked)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 0, 0, 255));
	}
	else
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 150, 0, 255));
	}
}

void CUi_BossHpBar::Render_BeginApollon()
{
	if (!m_bBigShaked)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0, 0, 255, 255));
	}
	else
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(150, 0, 255, 255));
	}
}

HRESULT CUi_BossHpBar::Render()
{

	switch (m_eFocusType)
	{
	case Client::BossType::Beholder:
		Render_Beholder();
		break;
	case Client::BossType::Artemis:
		Render_Artemis();
		break;
	case Client::BossType::Apollon:
		Render_Apollon();
		break;
	default:
		assert(false);
		break;
	}

	Render_Begin();

	m_pTransformCom->Bind_WorldMatrix();
	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	Render_End();

	return S_OK;
}

void CUi_BossHpBar::Render_Beholder()
{
	m_pBackGroundTransformCom->Bind_WorldMatrix();
	m_pBackGroundTextureCom[(_uint)BossType::Beholder]->Bind_Texture(0);
	m_pBackGroundVIBufferCom->Render();


	m_pNameTransformCom->Bind_WorldMatrix();
	m_pNameTextureCom[(_uint)BossType::Beholder]->Bind_Texture(0);
	m_pNameVIBufferCom->Render();
}

void CUi_BossHpBar::Render_Artemis()
{
	m_pBackGroundTransformCom->Bind_WorldMatrix();
	m_pBackGroundTextureCom[(_uint)BossType::Artemis]->Bind_Texture(0);
	m_pBackGroundVIBufferCom->Render();


	m_pNameTransformCom->Bind_WorldMatrix();
	m_pNameTextureCom[(_uint)BossType::Artemis]->Bind_Texture(0);
	m_pNameVIBufferCom->Render();
}

void CUi_BossHpBar::Render_Apollon()
{
	m_pBackGroundTransformCom->Bind_WorldMatrix();
	m_pBackGroundTextureCom[(_uint)BossType::Apollon]->Bind_Texture(0);
	m_pBackGroundVIBufferCom->Render();


	m_pNameTransformCom->Bind_WorldMatrix();
	m_pNameTextureCom[(_uint)BossType::Apollon]->Bind_Texture(0);
	m_pNameVIBufferCom->Render();
}

void CUi_BossHpBar::Render_End()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
}

HRESULT CUi_BossHpBar::Initialize_Active()
{
	Initialize_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(nullptr);
	
	for (size_t i = 0; i <(_uint)BossType::End; ++i)
	{
		m_iOldPercent[i] = 10;
	}

	m_eFocusType = BossType::Beholder;

	return S_OK;
}

void CUi_BossHpBar::Initialize_Set_ActiveTime()
{
	
}

void CUi_BossHpBar::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 750;
	m_UiDesc.m_fSizeY = 25;

	m_BackDesc.m_fSizeX = 750;
	m_BackDesc.m_fSizeY = 25;

	m_NameDesc.m_fSizeX = 320;
	m_NameDesc.m_fSizeY = 40;
}

void CUi_BossHpBar::Initialize_Set_Speed()
{
}

void CUi_BossHpBar::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 0;
	m_UiDesc.m_fY = 280;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));


	_float3 BackScale = { m_BackDesc.m_fSizeX, m_BackDesc.m_fSizeY, 1.f };

	m_BackDesc.m_fX = m_UiDesc.m_fX;
	m_BackDesc.m_fY = m_UiDesc.m_fY;

	m_pBackGroundTransformCom->Set_Scale(BackScale);
	m_pBackGroundTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_BackDesc.m_fX, m_BackDesc.m_fY, 0.f));


	_float3 NameScale = { m_NameDesc.m_fSizeX, m_NameDesc.m_fSizeY, 1.f };

	m_NameDesc.m_fX = 0;
	m_NameDesc.m_fY = m_UiDesc.m_fY + 40;

	m_pNameTransformCom->Set_Scale(NameScale);
	m_pNameTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_NameDesc.m_fX, m_NameDesc.m_fY, 0.f));
}

HRESULT CUi_BossHpBar::Add_Components(void* pArg)
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
		(CComponent**)&m_pBackGroundVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pBackGroundTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pNameVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pNameTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUi_BossHpBar::Add_Texture(void* pArg)
{
	// beholder

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Beholder_BackGround_Texture"),
		(CComponent**)&m_pBackGroundTextureCom[(_uint)BossType::Beholder])))
		return E_FAIL;
	
	
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Name_Beholder_Texture"),
		(CComponent**)&m_pNameTextureCom[(_uint)BossType::Beholder])))
		return E_FAIL;
	
	// artemis

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Artemis_BackGround_Texture"),
		(CComponent**)&m_pBackGroundTextureCom[(_uint)BossType::Artemis])))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Name_Artemis_Texture"),
		(CComponent**)&m_pNameTextureCom[(_uint)BossType::Artemis])))
		return E_FAIL;

	//Apollon

	
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Apollon_BackGround_Texture"),
		(CComponent**)&m_pBackGroundTextureCom[(_uint)BossType::Apollon])))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Name_Apollon_Texture"),
		(CComponent**)&m_pNameTextureCom[(_uint)BossType::Apollon])))
		return E_FAIL;

	//hpbar(mutual)

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_BossHpBar_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_BossHpBar::Shaking(_float fTimeDelta)
{
	if (!m_bBigShaked)
	{
		switch (m_eFocusType)
		{
		case Client::BossType::Beholder:
			if (m_pBeholder == nullptr || m_pBeholder->Get_RecentHitTime() > 0.4)
				return;
			break;
		case Client::BossType::Artemis:
			if (m_pArtemis == nullptr || m_pArtemis->Get_RecentHitTime() > 0.4)
				return;
			break;
		case Client::BossType::Apollon:
			break;
		default:
			assert(false);
			break;
		}
	}
	m_fShakingTime += fTimeDelta;
	m_fShakingGap += fTimeDelta;
	if (m_fShakingTime > 0.8f)
	{
		m_fShakingTime = 0;
		m_bBigShaked = false;
	}

	if (m_fShakingGap > 0.05)
	{
		m_fShakingGap = 0;
		_float3 ShakePos = m_OriginPos;
		if (m_bBigShaked)
		{
			ShakePos += {CGame_Manager::Get_Instance()->Object_Shake(8).x, CGame_Manager::Get_Instance()->Object_Shake(10).y, 0};
		}
		else
		{
			ShakePos += {CGame_Manager::Get_Instance()->Object_Shake(4).x, CGame_Manager::Get_Instance()->Object_Shake(5).y, 0};
		}
		m_pTransformCom->Set_Position(ShakePos);
		m_pBackGroundTransformCom->Set_Position(ShakePos);
	}
}

void CUi_BossHpBar::Set_HpTextureIndex()
{
	_float HpMax = { 1 };
	_float Hp = { 0 };
	switch (m_eFocusType)
	{
	case Client::BossType::Beholder:
		if (m_pBeholder == nullptr)
			return;
		else
		{
			HpMax = m_pBeholder->Get_MaxHp();
			Hp = m_pBeholder->Get_Hp();
		}
		break;
	case Client::BossType::Artemis:
		if (m_pArtemis == nullptr)
			return;
		else
		{
			HpMax = m_pArtemis->Get_MaxHp();
			Hp = m_pArtemis->Get_Hp();
		}
		break;
	case Client::BossType::Apollon:
		if (m_pApollon == nullptr)
			return;
		else
		{
			HpMax = m_pApollon->Get_MaxHp();
			Hp = m_pApollon->Get_Hp();
		}
		break;
	default:
		assert(false);
	}

	_uint HpPercent = _uint((Hp / HpMax) * 10);
	if (HpPercent > m_pTextureCom->Get_MaxTextureNum())
		assert(false);

	if (m_iOldPercent[(_uint)m_eFocusType] != HpPercent)
	{
		m_bBigShaked = true;
		m_iOldPercent[(_uint)m_eFocusType] = HpPercent;
	}
	m_iTexture_Index = HpPercent;
}

void CUi_BossHpBar::LockHpBar()
{
	if (m_eFocusType == BossType::Beholder)
	{
		if (m_iTexture_Index == 7)
		{
			m_bLockHpBar = true;
		}
		else
		{
			m_bLockHpBar = false;
		}
	}
}

void CUi_BossHpBar::Decision_Type()
{
	_float BeholderHitTime = { 9999 };
	_float ArtemisHitTime = { 9999 };
	_float ApollonHitTime = { 9999 };
	if (m_pBeholder)
	{
		BeholderHitTime = m_pBeholder->Get_RecentHitTime();
	}
	if (m_pArtemis)
	{
		ArtemisHitTime = m_pArtemis->Get_RecentHitTime();
	}
	if (m_pApollon)
	{
		ApollonHitTime = m_pApollon->Get_RecentHitTime();
	}


	if (BeholderHitTime <= ArtemisHitTime && BeholderHitTime <= ApollonHitTime)
	{
		m_eFocusType = BossType::Beholder;
	}
	else if(ArtemisHitTime <= ApollonHitTime)
	{
		m_eFocusType = BossType::Artemis;
	}
	else
	{
		m_eFocusType = BossType::Apollon;
	}
}

void CUi_BossHpBar::Check_BossDead()
{
	if (m_pBeholder && m_pBeholder->Get_Hp() <= 0)
	{
		Safe_Release(m_pBeholder);
	}

	if (m_pArtemis && m_pArtemis->Get_Hp() <= 0)
	{
		Safe_Release(m_pArtemis);
	}

	if (m_pApollon && m_pApollon->Get_Hp() <= 0)
	{
		Safe_Release(m_pApollon);
	}

}

void CUi_BossHpBar::Set_Beholder(CBeholder* pBeholder)
{
	if (pBeholder == nullptr)
		assert(false);

	if (m_pBeholder)
	{
		assert(false);
	}
	else
	{
		m_pBeholder = pBeholder;
		Safe_AddRef(m_pBeholder);
	}
}

void CUi_BossHpBar::Set_Artemis(CArtemis* pArtemis)
{
	if (pArtemis == nullptr)
		assert(false);

	if (m_pArtemis)
	{
		assert(false);
	}
	else
	{
		m_pArtemis = pArtemis;
		Safe_AddRef(m_pArtemis);
	}
}

void CUi_BossHpBar::Set_Apollon(CApollo* pApollon)
{
	if (pApollon == nullptr)
		assert(false);

	if (m_pApollon)
	{
		assert(false);
	}
	else
	{
		m_pApollon = pApollon;
		Safe_AddRef(m_pApollon);
	}
}

CUi_BossHpBar* CUi_BossHpBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_BossHpBar* pInstance = new CUi_BossHpBar(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_BossHpBar Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_BossHpBar::Free()
{
	__super::Free();

	Safe_Release(m_pBeholder);
	Safe_Release(m_pArtemis);
	Safe_Release(m_pApollon);
	Safe_Release(m_pBackGroundVIBufferCom);
	Safe_Release(m_pBackGroundTransformCom);
	Safe_Release(m_pNameTransformCom);
	Safe_Release(m_pNameVIBufferCom);

	for (size_t i = 0; i < (_uint)BossType::End; ++i)
	{
		Safe_Release(m_pBackGroundTextureCom[i]);
		Safe_Release(m_pNameTextureCom[i]);
	}
}
