#include "CUi_Execution_Show.h"

CUi_Execution_Show::CUi_Execution_Show(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Execution_Show::CUi_Execution_Show(const CUi_Execution_Show& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Execution_Show::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUi_Execution_Show::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Execution_Show::PriorityTick(_float fTimeDelta)
{
}

void CUi_Execution_Show::Tick(_float fTimeDelta)
{
    m_fActiveTime-= fTimeDelta;
    if (m_fActiveTime < 0)
    {
        m_bActive = false;
    }

    WeaponMove(fTimeDelta);
    WeaponScaling(fTimeDelta);
    WeaponBlink(fTimeDelta);
}

void CUi_Execution_Show::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Execution_Show::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();

    if (FAILED(m_pBackGroundTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pBackGroundTextureCom->Bind_Texture(m_iTexture_Index);
    m_pBackGroundVIBufferCom->Render();

    if (m_bWeaponBlink)
    {
        if (FAILED(m_pWeaponTransformCom->Bind_WorldMatrix()))
            return E_FAIL;

        m_pWeaponTextureCom->Bind_Texture(0);
        m_pWeaponVIBufferCom->Render();
    }
    return S_OK;
}

HRESULT CUi_Execution_Show::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);

    m_fScalingTime = { 0 };
    m_fMoveTime = { 0 };
    m_fWeaponBlinkTime = { 0 };
    m_bWeaponBlink = { false };

    return S_OK;
}

void CUi_Execution_Show::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 5.f;
}

void CUi_Execution_Show::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = 200;
    m_UiDesc.m_fSizeY = 50;

    m_Weapon.m_fSizeX = 80;
    m_Weapon.m_fSizeY = 250;

    m_WeaponBackGround.m_fSizeX = 100;
    m_WeaponBackGround.m_fSizeY = 100;

}

void CUi_Execution_Show::Initialize_Set_Speed()
{
    m_pWeaponTransformCom->Set_Speed(1500);
}

void CUi_Execution_Show::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = -250;

    m_pTransformCom->Set_Scale(Scale);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

    _float3 WeaponScale = { m_Weapon.m_fSizeX, m_Weapon.m_fSizeY, 1 };
    m_Weapon.m_fX = 0;
    m_Weapon.m_fY = -500;

    m_pWeaponTransformCom->Set_Scale(WeaponScale);
    m_pWeaponTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_Weapon.m_fX, m_Weapon.m_fY, 0.f));

    _float3 WeaponBackScale = { m_WeaponBackGround.m_fSizeX, m_WeaponBackGround.m_fSizeY, 1 };
    m_WeaponBackGround.m_fX = 0;
    m_WeaponBackGround.m_fY = -150;

    m_pBackGroundTransformCom->Set_Scale(WeaponBackScale);
    m_pBackGroundTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_WeaponBackGround.m_fX, m_WeaponBackGround.m_fY, 0.f));
    m_pBackGroundTransformCom->Rotation_XYZ(_float3{ 0, 0, -45 });
}

HRESULT CUi_Execution_Show::Add_Components(void* pArg)
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
        (CComponent**)&m_pWeaponVIBufferCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("Transform_Default"),
        (CComponent**)&m_pWeaponTransformCom)))
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

    if (FAILED(Add_Texture(nullptr)))
        return E_FAIL;


    return S_OK;
}

HRESULT CUi_Execution_Show::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Execution_BackGround_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Execution_Show_Texture"),
        (CComponent**)&m_pWeaponTextureCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Execution_WeaponBackGround_Texture"),
        (CComponent**)&m_pBackGroundTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_Execution_Show::WeaponMove(_float fTimeDelta)
{
    m_fMoveTime += fTimeDelta;
    if (m_fMoveTime < 0.3f)
    {
        m_pWeaponTransformCom->Go_Up(fTimeDelta);
    }
    else
    {
        m_pWeaponTransformCom->Set_Position(m_WeaponOriginPos);
    }
}

void CUi_Execution_Show::WeaponScaling(_float fTimeDelta)
{
    m_fScalingTime += fTimeDelta;
    if (m_fScalingTime > 0.6f && m_fScalingTime < 0.9f)
    {
        _float3 ScaleDown = { 0.97f, 0.97f, 1.f };
        m_pWeaponTransformCom->Multiply_Scale(ScaleDown);
    }
    else if (m_fScalingTime > 0.9f)
    {
        m_pWeaponTransformCom->Set_Scale(m_WeaponOriginScale);
    }
}

void CUi_Execution_Show::WeaponBlink(_float fTimeDelta)
{
    m_fWeaponBlinkTime += fTimeDelta;

    if (m_fWeaponBlinkTime > 0.3f)
    {
        m_fWeaponBlinkTime = 0.f;
        m_bWeaponBlink = false;
    }
    else
    {
        m_bWeaponBlink = true;
    }
}

CUi_Execution_Show* CUi_Execution_Show::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Execution_Show* pInstance = new CUi_Execution_Show(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Execution_Show Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Execution_Show::Free()
{
    __super::Free();
	Safe_Release(m_pWeaponTextureCom);
	Safe_Release(m_pWeaponTransformCom);
	Safe_Release(m_pWeaponVIBufferCom);
    Safe_Release(m_pBackGroundTextureCom);
    Safe_Release(m_pBackGroundTransformCom);
    Safe_Release(m_pBackGroundVIBufferCom);
}
