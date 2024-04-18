#include "Door.h"

#include "GameInstance.h"

#include "Player.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CDoor::CDoor(const CDoor& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}

void CDoor::PriorityTick(_float fTimeDelta)
{
}

void CDoor::Tick(_float fTimeDelta)
{
	if (m_bOpening)
	{
		m_fTheta -= 700.f * (_float)m_eDir * fTimeDelta;

		if (fabsf(m_fTheta - m_fStartTheta) > 85.f)
		{
			m_bOpening = false;
			m_pBoxCollider->Set_Active(false);
			return;
		}

		_quat quat;
		D3DXQuaternionRotationAxis(&quat, &_float3(0.f, 1.f, 0.f), D3DXToRadian(m_fTheta));

		auto x = *D3DXMatrixTranslation(&_float4x4(), m_vPivot.x, m_vPivot.y, m_vPivot.z);
		auto y = *D3DXMatrixRotationQuaternion(&_float4x4(), &quat);
		auto z = *D3DXMatrixTranslation(&_float4x4(), -m_vPivot.x, -m_vPivot.y, -m_vPivot.z);

		_float4x4 PivotMatrix = x * y * z;

		m_pTransformCom->Set_WorldMatrix(PivotMatrix * m_OriginMatrix);
	}

	//m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

}

void CDoor::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDoor::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBuffer->Render();

	m_pBoxCollider->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

HRESULT CDoor::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer = dynamic_cast<CVIBuffer_Door*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Door_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Door_Textures"), TEXT("Door_Textures"), nullptr));
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

void CDoor::OnCollisionEnter(CGameObject* pOther)
{
	static_cast<CPlayer*>(pOther)->Kick();
	m_bOpening = true;
	m_pBoxCollider->Set_Active(false);
	Open_Door();
}

void CDoor::Open_Door()
{
	m_OriginMatrix = m_pTransformCom->Get_WorldMatrix();

	_float fMagicNumber = 0.492f * m_pTransformCom->Get_Scale().x;

	_float3 vRight = { 1.0f, 0.f, 0.f };
	m_vPivot = vRight * fMagicNumber;
	m_fStartTheta = m_fTheta = 0.f;

	m_bOpening = true;
}

_bool CDoor::isDirection_Right()
{
	if (RIGHT == m_eDir)
		return true;

	return false;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor* pInstance = new CDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
