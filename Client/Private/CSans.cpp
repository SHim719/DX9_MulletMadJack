#include "CSans.h"
#include "FPS_Camera.h"
#include "CBone_Spawner.h"
#include "PlayerManager.h"
#include "CGaster_Spawner.h"
#include "Layer.h"
#include "CGame_Manager.h"


CSans::CSans(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPawn(pGraphic_Device)
{
}

CSans::CSans(const CSans& rhs)
	:CPawn(rhs)
{
}

HRESULT CSans::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSans::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());
    m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
    Safe_AddRef(m_pTarget);

    _float3 Scale = { 2.f, 2.f, 2.f };
    m_pTransformCom->Set_Scale(Scale);
    m_pBodyTransformCom->Set_Scale(Scale);
    m_pBoxCollider->Set_Scale({ 1.f, 1.f, 1.f });

    _float3 Pos = { 0.f, 1.f, 6.f };
    m_pTransformCom->Set_Position(Pos);
    m_pBodyTransformCom->Set_Position(Pos);

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_UseGravity(false);

    m_pBoneSpawner = CBone_Spawner::Create();
    m_pGasterSpawner = CGaster_Spawner::Create();

    m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_TextBack");

    if (m_pPlayerTransformCom == nullptr)
    {
        m_pPlayerTransformCom = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform();
        Safe_AddRef(m_pPlayerTransformCom);
    }

    return S_OK;
}

void CSans::PriorityTick(_float fTimeDelta)
{
}

void CSans::Tick(_float fTimeDelta)
{
    SubPatternTime(fTimeDelta);
    State_Pattern();

    if (m_Turn == SansTurnBased::SansText)
    {
        Set_TextLength(fTimeDelta);
        Set_Text();
    }
    Adjust_BodyPos(fTimeDelta);
    m_fTransParentTime -= fTimeDelta;
}

void CSans::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

void CSans::RenderBegin()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

HRESULT CSans::Render()
{
    if (m_fTransParentTime <= 0)
    {
        RenderBegin();

        if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
            return E_FAIL;

        if (FAILED(m_pLegTextureCom->Bind_Texture(0)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        RenderBody();

        RenderEnd();
    }
	return S_OK;
}

void CSans::RenderBody()
{
    m_pBodyTransformCom->Bind_WorldMatrix();

    m_pBodyTextureCom->Bind_Texture(0);
        
    m_pVIBodyBufferCom->Render();
}

void CSans::RenderEnd()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSans::SetTurn(SansTurnBased Turn)
{
    m_Turn = Turn;

    if (Turn == SansTurnBased::SansText)
    {
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_TextBack");
        ++m_iSansTextCount;
        m_fTextLengthTime = 0;
        m_iSansTextLength = 0;
    }
}

void CSans::TransParent()
{
    m_fTransParentTime = 0.5f;
	m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(),
		L"SansMiss", L"Prototype_CSansMiss", &m_pTransformCom->Get_Pos());  
}

HRESULT CSans::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>
        (Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>
        (Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pVIBodyBufferCom = dynamic_cast<CVIBuffer_Rect*>
        (Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBufferBody")));

    m_pBodyTransformCom = dynamic_cast<CTransform*>
        (Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("TransformBody")));


    m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

    CBoxCollider::BOXCOLLISION_DESC pDesc;
    pDesc.vScale = { 1.f, 1.f, 1.f };

    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

    return S_OK;
}

HRESULT CSans::Add_Textures()
{
    m_pLegTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Texture_Sans_Leg")));
    m_pBodyTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Texture_Sans_Body")));

    return S_OK;
}

void CSans::SubPatternTime(_float fTimeDelta)
{
    m_fPatternTime -= fTimeDelta;
}

void CSans::Set_Pattern0Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 1:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 2:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 3:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 4:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 5:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansTurn);
        ++m_iPatternCount;
        m_iDetailPatternCount = 0;
        break;
    default:  
        break;
    }
}

void CSans::Set_Pattern1Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.1f; 
        break;
    case 1:
        m_fPatternTime = 1.2f;
        break;
    case 2:
        m_fPatternTime = 1.2f;
        break;
    case 3:
        m_fPatternTime = 1.2f;
        break;
    case 4:
        m_fPatternTime = 1.2f;
        break;
    case 5:
        m_fPatternTime = 2.5f;
        break;
    case 6:
        m_fPatternTime = 2.5f;
        break;
    case 7:
        m_fPatternTime = 2.5f;
        break;
    case 8:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 9:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern2Time()
{
    _float3 Pos{};
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.f;
        break;
    case 1:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f , 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 2:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f , 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 3:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f , 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 4:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f , 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 5:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern3Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.f;
        break;
    case 1:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 2:
        m_fPatternTime = 1.5f;
        break;
    case 3:
        m_fPatternTime = 1.5f;
        break;
    case 4:
        m_fPatternTime = 1.5f;
        break;
    case 5:
        m_fPatternTime = 1.5f;
        break;
    case 6:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern4Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.f;
        break;
    case 1:
        m_fPatternTime = 1.5f;      
        break;
    case 2:
        m_fPatternTime = 1.5f;
        break;
    case 3:
        m_fPatternTime = 1.5f;
        break;
    case 4:
        m_fPatternTime = 1.5f;
        break;
    case 5:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern5Time()
{
    _float3 Pos{};
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.f;
        break;
    case 1:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { -1.5f, 3.1f, 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 2:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f, 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 3:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 0.f, 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 4:
        m_fPatternTime = 2.2f;
        m_pGameInstance->Set_Ui_ActiveState(L"CUi_Sans_Black");
        Pos = { 0.f, 1.6f, 0.f };
        m_pPlayerTransformCom->Set_Pos(Pos);
        break;
    case 5:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern14Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 1:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 2:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 3:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 4:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 5:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 6:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 7:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 8:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 9:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 10:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 11:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 12:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 13:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 14:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern17Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 1:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 2:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 3:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 4:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 5:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 6:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 7:
        m_fPatternTime = 1.5f;
        break; 
    case 8:
        m_fPatternTime = 1.5f;
        break;
    case 9:
        m_fPatternTime = 1.5f;
        break;
    case 10:
        m_fPatternTime = 1.5f;
        break;
    case 11:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_Pattern18Time()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 1:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 2:
        m_fPatternTime = 0;
        SetTurn(SansTurnBased::SansText);
        break;
    case 3:
        m_fPatternTime = 1.5f;
        break;
    case 4:
        m_fPatternTime = 1.5f;
        break;
    case 5:
        m_fPatternTime = 1.5f;
        break;
    case 6:
        m_fPatternTime = 1.5f;
        break;
    case 7:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Set_PatternDefaultTime()
{
    switch (m_iDetailPatternCount)
    {
    case 0:
        m_fPatternTime = 1.f;
        break;
    case 1:
        m_fPatternTime = 1.5f;
        break;
    case 2:
        m_fPatternTime = 1.5f;
        break;
    case 3:
        m_fPatternTime = 1.5f;
        break;
    case 4:
        m_fPatternTime = 1.5f;
        break;
    case 5:
        Add_SansPattern();
        break;
    default:
        break;
    }
}

void CSans::Add_SansPattern()
{
    CLayer* pBoneLayer = m_pGameInstance->Find_Layer(LEVEL_SANS, L"Layer_Bone");
    CLayer* pGasterLayer = m_pGameInstance->Find_Layer(LEVEL_SANS, L"Layer_Gaster");

    if (pBoneLayer)
    {
        list<CGameObject*>& BoneLayerObjects = pBoneLayer->Get_GameObjects();
        for (auto iter : BoneLayerObjects)
            iter->Set_Destroy(true);
    }
    if (pGasterLayer)
    {
        list<CGameObject*>& GasterLayerObjects = pGasterLayer->Get_GameObjects();
        for (auto iter : GasterLayerObjects)
            iter->Set_Destroy(true);
    }

    ++m_iPatternCount;
    m_iDetailPatternCount = 0;

    SetTurn(SansTurnBased::PlayerTurn);
}

void CSans::State_Pattern()
{
    if (m_fPatternTime <= 0 && m_Turn == SansTurnBased::SansTurn)
    {
        switch (m_iPatternCount)
        {
        case 0:
            Set_Pattern0Time();
            break;
        case 1:
            Set_Pattern1Time();
            break;
        case 2:
            Set_Pattern2Time();
            break;
        case 3:
            Set_Pattern3Time();
            break;
        case 4:
            Set_Pattern4Time();
            break;
        case 5:
            Set_Pattern5Time();
            break;
        case 6:
            Set_PatternDefaultTime();
            break;
        case 7:
            Set_PatternDefaultTime();
            break;
        case 8:
            Set_PatternDefaultTime();
            break;
        case 9:
            Set_PatternDefaultTime();
            break;
        case 10:
            Set_PatternDefaultTime();
            break;
        case 11:
            Set_PatternDefaultTime();
            break;
        case 12:
            Set_PatternDefaultTime();
            break;
        case 13:
            Set_PatternDefaultTime();
            break;
        case 14:
            Set_Pattern14Time();
            break;
        case 15:
            Set_PatternDefaultTime();
            break;
        case 16:
            Set_PatternDefaultTime();
            break;
        case 17:
            Set_Pattern17Time();
            break;
        case 18:
            Set_Pattern18Time();
            break;
        case 19:
            Set_PatternDefaultTime();
            break;
        case 20:
            Set_PatternDefaultTime();
            break;
        case 21:
            Set_PatternDefaultTime();
            break;
        case 22:
            Set_PatternDefaultTime();
            break;
        case 23:
            Set_PatternDefaultTime();
            break;
        default:
            break;
        }
        cout << m_iPatternCount << endl;
        m_pBoneSpawner->Spawn(m_iPatternCount, m_iDetailPatternCount);
        m_pGasterSpawner->Spawn(m_iPatternCount, m_iDetailPatternCount);
        ++m_iDetailPatternCount;
    }
}

void CSans::Set_Text()
{
    CText::Text_Info* Info = CGame_Manager::Get_Instance()->Get_Text(TextType::Sans, m_iSansTextCount);
    Info->Rect = { 300, 20, 800, 120 };
    Info->RGBA = D3DCOLOR_RGBA(0, 0, 0, 255);
    Info->Length = m_iSansTextLength;
    CGame_Manager::Get_Instance()->Print_Text_Sans(Info);
}

void CSans::Set_TextLength(_float fTimeDelta)
{
    m_fTextLengthTime += fTimeDelta;
    if (m_fTextLengthTime >= 0.5)
    {
        ++m_iSansTextLength;
    }
}

void CSans::Adjust_BodyPos(_float fTimeDelta)
{
    m_fBodyPosTime += fTimeDelta;
    if (m_fBodyPosTime > 0.1)
    {
        m_fBodyPosTime = 0;
        ++m_iSansIndex;
        if (m_iSansIndex == 12)
            m_iSansIndex = 0;

        _uint Index = m_iSansIndex % 12;
        _float3 BodyPos = m_pBodyTransformCom->Get_Pos();
        switch (Index)
        {
        case 0:
            BodyPos.y += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 1:
            BodyPos.x -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 2:
            BodyPos.y -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 3:
            BodyPos.y -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 4:
            BodyPos.x += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 5:
            BodyPos.y += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 6:
            BodyPos.y += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 7:
            BodyPos.x += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 8:
            BodyPos.y -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 9:
            BodyPos.y -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 10:
            BodyPos.x -= 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        case 11:
            BodyPos.y += 0.01f;
            m_pBodyTransformCom->Set_Position(BodyPos);
            break;
        default:
            break;
        }
    }
}

CSans* CSans::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSans* pInstance = new CSans(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CSans"));

        Safe_Release(pInstance);
    }
    
    return pInstance;
}

CGameObject* CSans::Clone(void* pArg)
{
    CSans* pInstance = new CSans(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CSansClone"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSans::Free()
{
	__super::Free();

    Safe_Release(m_pPlayerTransformCom);
    Safe_Release(m_pVIBodyBufferCom);
    Safe_Release(m_pBodyTransformCom);
    Safe_Release(m_pGasterSpawner);
    Safe_Release(m_pBoneSpawner);
    Safe_Release(m_pTarget);
    Safe_Release(m_pRigidbody);
    Safe_Release(m_pBodyTextureCom);
    Safe_Release(m_pLegTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
}
