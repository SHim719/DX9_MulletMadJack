#include "Katana_Effect.h"

CKatana_Effect::CKatana_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CKatana_Effect::CKatana_Effect(const CKatana_Effect& rhs)
	: CUi(rhs)
{
}

HRESULT CKatana_Effect::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CKatana_Effect::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.05f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CKatana_Effect::Initialize_Active()
{
	m_iTexture_Index = 0;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CKatana_Effect::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CKatana_Effect::Tick(_float fTimeDelta)
{
	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Katana_Effect"), false);
		AnimationDelayReset();
	}

	int m_iSlashCnt = CPlayer_Manager::Get_Instance()->Get_RealSlashCount();
	if (m_iSlashCnt == 0) {
		m_fRotation = { 0.f, 0.f, 90.f };
	}
	else if (m_iSlashCnt == 1) {
		m_fRotation = { 0.f, 0.f, 45.f };
	}
	else if (m_iSlashCnt == 2) {
		m_fRotation = { 0.f, 0.f, 135.f };
	}


	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CKatana_Effect::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX , m_UiDesc.m_fY, 0.f));
}
HRESULT CKatana_Effect::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CKatana_Effect::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(0, 0);
	Set_Divide(1.5f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CKatana_Effect::Initialize_Set_Speed()
{
}

void CKatana_Effect::Default_Set_LifeTime()
{
}

void CKatana_Effect::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 840;
	m_UiDesc.m_fSizeY = 1514;
}

POINT CKatana_Effect::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 2048, 2048 };
}

_float2 CKatana_Effect::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CKatana_Effect::Add_Components(void* pArg)
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


HRESULT CKatana_Effect::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Katana_Slash_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CKatana_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKatana_Effect* pInstance = new CKatana_Effect(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CKatana_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CKatana_Effect::Free()
{
	__super::Free();
}
