#include "CUi_GreenCross.h"

CUi_GreenCross::CUi_GreenCross(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_GreenCross::CUi_GreenCross(const CUi_GreenCross& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_GreenCross::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();
	return S_OK;
}

HRESULT CUi_GreenCross::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(pArg);

	return S_OK;
}

void CUi_GreenCross::PriorityTick(_float fTimeDelta)
{
}

void CUi_GreenCross::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;

	if (m_fLifeTime < 0)
	{
		m_bDead = true;
	}

	if (m_bColorMode)
	{
		Adjust_Color(fTimeDelta);
	}
	else
	{
		Adjust_Alpha(fTimeDelta);
	}

	Move(fTimeDelta);
}

void CUi_GreenCross::LateTick(_float fTimeDelta)
{
}

void CUi_GreenCross::Begin_RenderState()
{

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
		D3DCOLOR_RGBA(m_iColor, 255, m_iColor, m_iAlpha));
	
}


HRESULT CUi_GreenCross::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	Begin_RenderState();
	m_pVIBufferCom->Render();
	End_RenderState();

	return S_OK;
}

void CUi_GreenCross::End_RenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}

HRESULT CUi_GreenCross::Add_Components(void* pArg)
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


	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_GreenCross::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_GreenCross_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_GreenCross::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 50;
	m_UiDesc.m_fSizeY = 50;
}

void CUi_GreenCross::Default_Set_LifeTime()
{
	m_fLifeTime = 1.5f;
}

void CUi_GreenCross::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Ui_Pos_Size_Rotation* Arg = (Ui_Pos_Size_Rotation*)pArg;
	m_UiDesc.m_fX = Arg->m_fX;
	m_UiDesc.m_fY = Arg->m_fY;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_GreenCross::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(100);
}

void CUi_GreenCross::Adjust_Color(_float fTimeDelta)
{
	m_fColorTime += fTimeDelta;
	if (m_fColorTime > 0.1)
	{
		m_fColorTime = 0;
		if (m_iColor <= 30)
		{
			m_iColor = 0;
			m_bColorMode = false;
		}
		else
		{
			m_iColor -= 30;
		}
	}
}

void CUi_GreenCross::Adjust_Alpha(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;
	if (m_fAlphaTime > 0.1)
	{
		m_fAlphaTime = 0;
		if (m_iAlpha <= 20)
		{
			m_iAlpha = 0;
		}
		else
		{
			m_iAlpha -= 20;
		}
	}
}

void CUi_GreenCross::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Up(fTimeDelta);
}

CUi_GreenCross* CUi_GreenCross::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_GreenCross* pInstance = new CUi_GreenCross(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_GreenCross Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_GreenCross::Clone(void* pArg)
{
	CUi_GreenCross* pInstance = new CUi_GreenCross(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_GreenCross_Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_GreenCross::Free()
{
	__super::Free();
}
