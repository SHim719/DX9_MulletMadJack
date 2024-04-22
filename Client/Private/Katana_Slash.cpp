#include "Katana_Slash.h"

CKatana_Slash::CKatana_Slash(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CKatana_Slash::CKatana_Slash(const CKatana_Slash& rhs)
	: CUi(rhs)
{
}

HRESULT CKatana_Slash::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CKatana_Slash::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.05f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CKatana_Slash::Initialize_Active()
{
	m_iTexture_Index = 0;
	m_fDelay = 0.5f;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);
	fSlashPos = { 0.f, 0.f };

	m_iRandomSlash = CPlayer_Manager::Get_Instance()->Get_SlashCount();
	
	return S_OK;


}

void CKatana_Slash::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CKatana_Slash::Tick(_float fTimeDelta)
{

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index = 0;
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };
	m_pTransformCom->Set_Scale(m_fScale);

	m_fDelay -= fTimeDelta;

	if (m_fDelay < 0.f) {
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Ui_Katana_Slash"), false);
		CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::IDLE);
	}

}

void CKatana_Slash::LateTick(_float fTimeDelta)
{
	_float2 fDisablePos = CPlayer_Manager::Get_Instance()->Get_TempDisablePosition_BothHand() * -5.f;
	_float2 fOffSet = { 0, 0 };

	if (m_iRandomSlash == 0) {
		fOffSet = { -350, 50 };
		fSlashPos.x += 13000.f * fTimeDelta;
		m_pTransformCom->Rotation_XYZ({ 0.f, 0.f, 90.f });
	}
	else if (m_iRandomSlash == 1) {
		fOffSet = { -550, 650 };
		fSlashPos.x += 13000.f * fTimeDelta;
		fSlashPos.y -= 13000.f * fTimeDelta;
		m_pTransformCom->Rotation_XYZ({ 0.f, 0.f, 45.f });
	}
	else if (m_iRandomSlash == 2) {
		fOffSet = { -550, -650 };
		fSlashPos.x += 13000.f * fTimeDelta;
		fSlashPos.y += 13000.f * fTimeDelta;
		m_pTransformCom->Rotation_XYZ({ 0.f, 0.f, 135.f });
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fOffSet.x + fSlashPos.x, m_UiDesc.m_fY + fOffSet.y + fDisablePos.y + fSlashPos.y, 0.f));

	

}

HRESULT CKatana_Slash::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	
	return S_OK;
}

void CKatana_Slash::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(0, 0);
	Set_Divide(1.f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CKatana_Slash::Initialize_Set_Speed()
{
}

void CKatana_Slash::Default_Set_LifeTime()
{
}

void CKatana_Slash::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 840;
	m_UiDesc.m_fSizeY = 1514;
}

POINT CKatana_Slash::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 2048, 2048 };
}

_float2 CKatana_Slash::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CKatana_Slash::Add_Components(void* pArg)
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


HRESULT CKatana_Slash::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Katana_Hit_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CKatana_Slash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKatana_Slash* pInstance = new CKatana_Slash(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CKatana_Slash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CKatana_Slash::Free()
{
	__super::Free();
}
