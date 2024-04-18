#include "Shotgun_Reload.h"
#include "PlayerManager.h"
CShotgun_Reload::CShotgun_Reload(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CShotgun_Reload::CShotgun_Reload(const CShotgun_Reload& rhs)
	: CUi(rhs)
{
}

HRESULT CShotgun_Reload::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CShotgun_Reload::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.025f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CShotgun_Reload::Initialize_Active()
{
	Set_Texture_Index(0);
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CShotgun_Reload::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CShotgun_Reload::Tick(_float fTimeDelta)
{

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.1f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	//Animation End
	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		CPlayer_Manager::Get_Instance()->Set_Player_AnimationType(CPlayer::ANIMATION_TYPE::IDLE);
		CPlayer_Manager::Get_Instance()->Reload_Magazine();
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CShotgun_Reload::LateTick(_float fTimeDelta)
{
	//_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1, 3, 3, 1, 2, 6);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y, 0.f));
}

HRESULT CShotgun_Reload::Render()
{
	//임시방편입니다..

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	//임시방편입니다..
	return S_OK;
}

void CShotgun_Reload::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(350, -200);
	Set_Divide(1.5f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CShotgun_Reload::Initialize_Set_Speed()
{
}

void CShotgun_Reload::Default_Set_LifeTime()
{
}

void CShotgun_Reload::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 2048;
	m_UiDesc.m_fSizeY = 2048;
}

POINT CShotgun_Reload::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 2048, 2048 };
}

_float2 CShotgun_Reload::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CShotgun_Reload::Add_Components(void* pArg)
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


HRESULT CShotgun_Reload::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Shotgun_Reload_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CShotgun_Reload::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShotgun_Reload* pInstance = new CShotgun_Reload(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CShotgun_Reload"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CShotgun_Reload::Free()
{
	__super::Free();
}
