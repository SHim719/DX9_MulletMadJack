#include "Shotgun.h"

CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CShotgun::CShotgun(const CShotgun& rhs)
	: CUi(rhs)
{
}

HRESULT CShotgun::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CShotgun::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.1f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CShotgun::Initialize_Active()
{
	m_iTexture_Index = 0;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CShotgun::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CShotgun::Tick(_float fTimeDelta)
{

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

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
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CShotgun::LateTick(_float fTimeDelta)
{
	_float2 fDisablePos = CPlayer_Manager::Get_Instance()->Get_TempDisablePosition_BothHand() * -1.f;

	if (CGameInstance::Get_Instance()->GetKey(eKeyCode::LShift)) {
		//Temp Rotate
		m_pTransformCom->Rotation_XYZ({ 0.f, 0.f, 30.f });
		_float2 fOffSet = { 150, 50 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fOffSet.x, m_UiDesc.m_fY + fOffSet.y + fDisablePos.y, 0.1f));
	}
	else {


		_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1.5f, 2, 3, 1, 2, 6);
		_float2 fLissajousRun = CPlayer_Manager::Get_Instance()->Get_Lissajous_Run(m_UiDesc.m_fX, m_UiDesc.m_fY);

		if (CPlayer_Manager::Get_Instance()->Get_Player_MoveState() == CPlayer::MOVE_STATE::MOVE)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x + fLissajousRun.x, m_UiDesc.m_fY + fLissajousPos.y + fLissajousRun.y + fDisablePos.y, 0.1f));
		else
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y + fDisablePos.y, 0.1f));
	}
}
HRESULT CShotgun::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CShotgun::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(210, -270);
	Set_Divide(2.0f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.1f));
}

void CShotgun::Initialize_Set_Speed()
{
}

void CShotgun::Default_Set_LifeTime()
{
}

void CShotgun::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 840;
	m_UiDesc.m_fSizeY = 1514;
}

POINT CShotgun::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 2048, 2048 };
}

_float2 CShotgun::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CShotgun::Add_Components(void* pArg)
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


HRESULT CShotgun::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Shotgun_Idle_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CShotgun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShotgun* pInstance = new CShotgun(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CShotgun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CShotgun::Free()
{
	__super::Free();
}
