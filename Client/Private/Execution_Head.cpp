#include "Execution_Head.h"
#include "GameInstance.h"

CExecution_Head::CExecution_Head(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CExecution_Head::CExecution_Head(const CExecution_Head& rhs)
	: CUi(rhs)
{
}

HRESULT CExecution_Head::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CExecution_Head::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.025f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CExecution_Head::Initialize_Active()
{
	m_iTexture_Index = 0;
	m_fDelayedAnimation = 0.15f;
	m_fDelayedAnimation2 = 0.1f;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CExecution_Head::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CExecution_Head::Tick(_float fTimeDelta)
{
	//m_fDivide -= fTimeDelta * 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.1f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		if(m_iTexture_Index == 5){
			m_fDelayedAnimation -= fTimeDelta;
			if(m_fDelayedAnimation < 0.f){
				CPlayer_Manager::Get_Instance()->Camera_Shake_Order(0.1f, 2000000.f);
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Knife"), true);

				m_pGameInstance->Play(L"Player_Execution_Weapon", false);
				m_pGameInstance->SetVolume(L"Player_Execution_Weapon", 0.3f);

				m_iTexture_Index++;
			}
		}else if(m_iTexture_Index == m_pTextureCom->Get_MaxTextureNum()){
			m_fDelayedAnimation2 -= fTimeDelta;
			if(m_fDelayedAnimation2 < 0.f){
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Head"), false);
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Body"), false);
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Neck"), false);
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Knife"), false);
				CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("CUi_Finish"), true);
				CPlayer_Manager::Get_Instance()->Set_PlayerHP_Add(15.f);
				CPlayer_Manager::Get_Instance()->Set_Action_Type(CPlayer_Manager::ACTION_NONE);
				POINT		ptWindow = { g_iWinSizeX >> 1, g_iWinSizeY >> 1 };
				ClientToScreen(g_hWnd, &ptWindow);
				SetCursorPos(ptWindow.x, ptWindow.y);

				CPlayer_Manager::Get_Instance()->Set_MouseLock(true);
				CPlayer_Manager::Get_Instance()->Set_Invincible(false);
				AnimationDelayReset();
			}
		}
		else {
			m_iTexture_Index++;
		}

		if (m_iTexture_Index == (m_pTextureCom->Get_MaxTextureNum() - 3))
			CPlayer_Manager::Get_Instance()->Camera_Shake_Order(0.2f, 2000000.f);
		AnimationDelayReset();
	}

	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Head"), false);
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Body"), false);
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Neck"), false);
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Knife"), false);
		// minheok check this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(L"CUi_Finish");
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CExecution_Head::LateTick(_float fTimeDelta)
{
	if (m_iTexture_Index == (m_pTextureCom->Get_MaxTextureNum() - 3) || m_iTexture_Index == (m_pTextureCom->Get_MaxTextureNum() - 2)) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + CGame_Manager::Get_Instance()->Object_Shake(20).x, m_UiDesc.m_fY + CGame_Manager::Get_Instance()->Object_Shake(20).y, 0.1f));
	}//_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1.5f, 2, 3, 1, 2, 6);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + CGame_Manager::Get_Instance()->Object_Shake(5).x , m_UiDesc.m_fY + CGame_Manager::Get_Instance()->Object_Shake(5).y, 0.1f));
}

HRESULT CExecution_Head::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 110);
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CExecution_Head::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(-20, 22);
	Set_Divide(1.3f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CExecution_Head::Initialize_Set_Speed()
{
}

void CExecution_Head::Default_Set_LifeTime()
{
}

void CExecution_Head::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 1024;
	m_UiDesc.m_fSizeY = 1024;
}

POINT CExecution_Head::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 1024,1024 };
}

_float2 CExecution_Head::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CExecution_Head::Add_Components(void* pArg)
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


HRESULT CExecution_Head::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Execution_Head_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CExecution_Head::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExecution_Head* pInstance = new CExecution_Head(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CExecution_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CExecution_Head::Free()
{
	__super::Free();
}
