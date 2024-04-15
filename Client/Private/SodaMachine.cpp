#include "SodaMachine.h"
#include "GameInstance.h"

#include "Soda.h"

#include "Player.h"

CSodaMachine::CSodaMachine(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSodaMachine::CSodaMachine(const CSodaMachine& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSodaMachine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSodaMachine::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CSodaMachine::PriorityTick(_float fTimeDelta)
{
}

void CSodaMachine::Tick(_float fTimeDelta)
{
	if (POURING == m_eState)
	{
		Pouring_Soda(fTimeDelta);
	}

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
}

void CSodaMachine::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSodaMachine::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBuffer_Machine->Render();

	m_pBoxCollider->Render();

	return S_OK;
}

HRESULT CSodaMachine::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer_Machine = dynamic_cast<CVIBuffer_Mesh*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_SodaMachine_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBuffer_Machine)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Soda_Textures"), TEXT("Soda_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}


void CSodaMachine::OnCollisionEnter(CGameObject* pOther)
{
	if (IDLE != m_eState)
		return;

	static_cast<CPlayer*>(pOther)->Kick();

	m_eState = POURING;
	m_iPourCount = 6;

	m_pBanner->Set_Destroy(true);
}


void CSodaMachine::Pouring_Soda(_float fTimeDelta)
{
	m_fPourTime += fTimeDelta;

	if (m_fPourTime >= 0.2f)
	{
		CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Soda", L"Prototype_Soda");
		pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vPourPos);
		static_cast<CBoxCollider*>(pObj->Find_Component(L"Collider"))->Update_BoxCollider(pObj->Get_Transform()->Get_WorldMatrix());

		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 vUp = { 0.f ,1.f, 0.f };

		_float fUpSpeed = _float(rand() % 3);
		_float fLookSpeed = _float(rand() % 1 + 1);
		static_cast<CRigidbody*>(pObj->Find_Component(L"Rigidbody"))->Set_Velocity(vLook * fLookSpeed + vUp * fUpSpeed);
		m_fPourTime = 0.f;
		m_iPourCount--;
	}

	if (0 == m_iPourCount)
		m_eState = BROKEN;
}

CSodaMachine* CSodaMachine::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSodaMachine* pInstance = new CSodaMachine(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSodaMachine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSodaMachine::Clone(void* pArg)
{
	CSodaMachine* pInstance = new CSodaMachine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSodaMachine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSodaMachine::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer_Machine);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}