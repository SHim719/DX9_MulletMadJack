#include "CBoneRoop.h"
#include "GameInstance.h"

CBoneRoop::CBoneRoop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBoneRoop::CBoneRoop(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBoneRoop::Initialize(void* pArg)
{
	BoneRoopArg* Arg = (BoneRoopArg*)pArg;
	m_ePos = Arg->Pos;
	m_fRoopGap = Arg->RoopGap;
	m_iFloor = Arg->Floor;
	m_fSpeed = Arg->Speed;
	m_fLifeTime = Arg->LifeTime;

	return S_OK;
}

void CBoneRoop::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	Roop(fTimeDelta);
	if (m_fLifeTime < 0)
	{
		m_bDestroyed = true;
	}

}

void CBoneRoop::Roop(_float fTimeDelta)
{
	m_fRoopTime += fTimeDelta;

	if (m_fRoopTime > m_fRoopGap)
	{
		m_fRoopTime = 0;
		SansBoneArg pArg;
		pArg.floor = m_iFloor;
		pArg.fSpeed = m_fSpeed;
		pArg.Pos = m_ePos;
		m_pGameInstance->Add_Clone(LEVEL_SANS, L"Layer_Bone", L"Prototype_CSans_Bone", &pArg);
	}

	if (m_fRoopGap == 0)
		m_bDestroyed = true;
}

CBoneRoop* CBoneRoop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoneRoop* m_pInstance = new CBoneRoop(pGraphic_Device);

    return m_pInstance;
}

CGameObject* CBoneRoop::Clone(void* pArg)
{
	CBoneRoop* m_pInstance = new CBoneRoop(*this);

	if (FAILED(m_pInstance->Initialize(pArg)))
	{
		
	}

	return m_pInstance;
}

void CBoneRoop::Free()
{
	__super::Free();
}
