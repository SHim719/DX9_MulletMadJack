#include "CUi_Floor.h"
#include "CUi_Floor_Part.h"
#include "GameInstance.h"


CUi_Floor::CUi_Floor(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Floor::CUi_Floor(const CUi_Floor& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Floor::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUi_Floor::Initialize(void* pArg)
{
  
    return S_OK;
}

HRESULT CUi_Floor::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    return S_OK;
}

void CUi_Floor::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor::Tick(_float fTimeDelta)
{
    Add_Time(fTimeDelta);
    Calculate_Call();
    Dead_Check();
    Cal_Blink(fTimeDelta);

    for (auto& iter : m_pPartlist)
        iter->Tick(fTimeDelta);
}

void CUi_Floor::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Floor::Render()
{
    if (Cal_Blink_Render(0.5f))
    {
        for(auto& iter : m_pPartlist)
            iter->Render();      
    }
    return S_OK;
}

void CUi_Floor::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 6.f;
    m_iCallCount = 0;
    m_fCallTime = 0;
    m_fBlink = 0;
}

void CUi_Floor::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
}

void CUi_Floor::Initialize_Set_Speed()
{
}

HRESULT CUi_Floor::Add_Components(void* pArg)
{
    return S_OK;
}

HRESULT CUi_Floor::Add_Texture(void* pArg)
{
    return S_OK;
}

void CUi_Floor::Cal_Blink(_float fTimeDelta)
{
    if (m_fActiveTime <= 4.f && 3.0f<=m_fActiveTime)
    {
        m_bBlink = true;
    }
    else
    {
        m_bBlink = false;
    }
}

bool CUi_Floor::Cal_Blink_Render(_float BlinkGap)
{
    if (m_bBlink)
    {
        if (m_fBlink > BlinkGap)
        {
            m_fBlink = 0.f;
            return true;
        }
        return false;
    }

    return true;
}

void CUi_Floor::SelectPart(_uint Number)
{
    switch (Number)
    {
    case (_uint)Part::F:
        ePart = Part::F;
        break;
    case (_uint)Part::L:
        ePart = Part::L;
        break;
    case (_uint)Part::First_O:
        ePart = Part::First_O;
        break;
    case (_uint)Part::Second_O:
        ePart = Part::Second_O;
        break;
    case (_uint)Part::R:
        ePart = Part::R;
        break;
    case (_uint)Part::Number1:
        ePart = Part::Number1;
        break;
    case (_uint)Part::Number2:
        ePart = Part::Number2;
        break;
    default:
        break;
    }
}

void CUi_Floor::Add_Time(_float fTimeDelta)
{
    m_fCallTime += fTimeDelta;
    m_fActiveTime -= fTimeDelta;
    m_fBlink += fTimeDelta;
}

void CUi_Floor::Calculate_Call()
{
    if (m_fCallTime > 0.1 && m_iCallCount < (_uint)Part::End)
    {
        SelectPart(m_iCallCount);
        m_pPartlist.emplace_back((CUi_Floor_Part*)m_pGameInstance->
            Add_Ui_PartClone(TEXT("CUi_Floor_Part"), &ePart));
        ++m_iCallCount;
        m_fCallTime = 0;
    }
}

void CUi_Floor::Dead_Check()
{
    auto iter = m_pPartlist.begin();
    for (; iter != m_pPartlist.end();)
    {
        if ((*iter)->Is_Dead())
        {
            Safe_Release(*iter);
            iter = m_pPartlist.erase(iter);
        }
        else
            ++iter;
    }

    if (m_fActiveTime < 0)
    {
        m_bActive = false;

    }

}

CUi_Floor* CUi_Floor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Floor* pInstance = new CUi_Floor(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_Floor Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Floor::Free()
{
    for (auto& iter : m_pPartlist)
        Safe_Release(iter);

    m_pPartlist.clear();

    __super::Free();
}
