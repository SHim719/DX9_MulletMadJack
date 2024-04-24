#include "CSans_Gaster.h"
#include "CGasterLaser.h"


CSans_Gaster::CSans_Gaster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSans_Gaster::CSans_Gaster(const CSans_Gaster& rhs)
	:CGameObject(rhs)
{
}

HRESULT CSans_Gaster::Initialize_Prototype()
{
	return S_OK;
}
HRESULT CSans_Gaster::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Texture()))
		return E_FAIL;

	Initialize_Arg(pArg);

	m_pTransformCom->Set_Speed(24);

	m_strTag = "Sans_Gaster";

	return S_OK;
}

void CSans_Gaster::Initialize_Arg(void* pArg)
{
	GasterArg* Arg = (GasterArg*)pArg;
	m_eFirePos = Arg->FirePos;
	m_iFloor = Arg->_floor;
	switch (m_eFirePos)
	{
	case SansGasterFirePos::Straight:
		Set_ArgStraightPos(Arg->Pos);
		SetState(GasterState::Go_Down);
		SetOriginState(GasterState::Go_Down);
		break;		  
	case SansGasterFirePos::BackWard:
		Set_ArgBackWardPos(Arg->Pos);
		SetState(GasterState::Go_Down);
		SetOriginState(GasterState::Go_Down);
		break;		 
	case SansGasterFirePos::Left:
		Set_ArgLeftPos(Arg->Pos);
		SetState(GasterState::Go_Left);
		SetOriginState(GasterState::Go_Left);
		break;		
	case SansGasterFirePos::Right:
		Set_ArgRightPos(Arg->Pos);
		SetState(GasterState::Go_Right);
		SetOriginState(GasterState::Go_Right);
		break;		 
	case SansGasterFirePos::Down:
		Set_ArgDownPos(Arg->Pos);
		SetState(GasterState::Go_Down);
		SetOriginState(GasterState::Go_Down);
		break;		  
	case SansGasterFirePos::Up:
		Set_ArgUpPos(Arg->Pos);
		SetState(GasterState::Go_Up);
		SetOriginState(GasterState::Go_Up);
		break;		 
	case SansGasterFirePos::End:
		break;
	default:
		break;
	}

}

void CSans_Gaster::PriorityTick(_float fTimeDelta)
{
}

void CSans_Gaster::Tick(_float fTimeDelta)
{
	m_fLife -= fTimeDelta;
	if (m_fLife < 0)
	{
		m_bDestroyed = true;
	}
	TextureSwitching(fTimeDelta);

	if (m_eState == GasterState::Laser)
	{
		m_fStateSwitching += fTimeDelta;
		if (m_fStateSwitching > 1.2)
		{
			SetStateSwitching();
		}
	}

	Move(fTimeDelta);
}

void CSans_Gaster::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

void CSans_Gaster::RenderBegin()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

HRESULT CSans_Gaster::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	RenderBegin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	RenderEnd();

	return S_OK;
}

void CSans_Gaster::RenderEnd()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSans_Gaster::OnTriggerEnter(CGameObject* pOther)
{
}

HRESULT CSans_Gaster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component
	(LEVEL_GAMEPLAY, TEXT("CSans_Gaster_Texture"), TEXT("CSans_Gaster_Texture")));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.25f, 0.25f, 0.5f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
	(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CSans_Gaster::Add_Texture()
{

	return S_OK;
}

void CSans_Gaster::TextureSwitching(_float fTimeDelta)
{
	m_fTextureSwitching += fTimeDelta;
	if (m_fTextureSwitching > 0.2)
	{
		if (m_eState != GasterState::Laser && !m_bCreateLaser)
		{
			++m_iTexture_Index;
			if (m_iTexture_Index >= m_pTextureCom->Get_MaxTextureNum())
			{
				SetStateLaser();
				m_iTexture_Index = m_pTextureCom->Get_MaxTextureNum();
			}
		}
		else
		{
			++m_iTexture_Index;
			if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
			{
				m_iTexture_Index = 4;
			}
		}

		m_fTextureSwitching = 0;
	}

}

void CSans_Gaster::Move(_float fTimeDelta)
{
	switch (m_eState)
	{
	case GasterState::Go_Down:
		m_pTransformCom->Go_Down(fTimeDelta);
		break;
	case GasterState::Go_Up:
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case GasterState::Go_Left:
		m_pTransformCom->Go_Left(fTimeDelta);
		break;
	case GasterState::Go_Right:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	case GasterState::Go_Straight:
		m_pTransformCom->Go_Straight(fTimeDelta);
		break;
	case GasterState::Go_BackWard:
		m_pTransformCom->Go_Backward(fTimeDelta);
		break;
	case GasterState::Laser:
		m_pTransformCom->Set_Position(m_OriginPos);
		break;
	case GasterState::End:
		break;
	default:
		break;
	}
}

void CSans_Gaster::SetStateLaser()
{
	m_eState = GasterState::Laser;
	m_bCreateLaser = true;
	GasterLaserArg pArg;
	pArg.FirePos = m_eFirePos;
	pArg.Pos = m_OriginPos;

	m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Layer_GasterLaser", 
		TEXT("Prototype_CGasterLaser"), &pArg);
}

void CSans_Gaster::SetStateSwitching()
{
	switch (m_eOriginState)
	{
	case GasterState::Go_Down:
		SetState(GasterState::Go_Down);
		break;
	case GasterState::Go_Up:
		SetState(GasterState::Go_Up);
		break;
	case GasterState::Go_Left:
		SetState(GasterState::Go_Down);
		break;
	case GasterState::Go_Right:
		SetState(GasterState::Go_Down);
		break;
	case GasterState::Go_Straight:
		SetState(GasterState::Go_Down);
		break;
	case GasterState::Go_BackWard:
		SetState(GasterState::Go_Down);
		break;
	case GasterState::Laser:
		break;
	case GasterState::End:
		break;
	default:
		break;
	}
}

void CSans_Gaster::Set_ArgStraightPos(SansGasterPos Pos)
{	
	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { -1.5f, 25.f, 4.f };
		m_OriginPos = { -1.5f, 1.f, 4.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { 0, 25.f, 4.f };
		m_OriginPos = { 0, 1.f, 4.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { 1.5f, 25.f, 4.f };
		m_OriginPos = { 1.5f, 1.f, 4.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}

	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		break;
	case 2:
		FloorAdjustPos.y += 1.5f;
		m_OriginPos.y += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 3:
		FloorAdjustPos.y += 3.f;
		m_OriginPos.y += 3.f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}

}

void CSans_Gaster::Set_ArgBackWardPos(SansGasterPos Pos)
{
	_float3 Rot = { 0, 180, 0 };
	m_pTransformCom->Rotation_XYZ(Rot);

	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { -1.5f, 25.f, -4.f };
		m_OriginPos = { -1.5f, 1.f, -4.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { 0, 25.f, -4.f };
		m_OriginPos = { 0, 1.f, -4.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { 1.5f, 25.f, -4.f };
		m_OriginPos = { 1.5f, 1.f, -4.f};
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}

	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		break;
	case 2:
		FloorAdjustPos.y += 1.5f;
		m_OriginPos.y += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 3:
		FloorAdjustPos.y += 3.f;
		m_OriginPos.y += 3.f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}

}

void CSans_Gaster::Set_ArgLeftPos(SansGasterPos Pos)
{
	_float3 Rot = { 0, 270, 0 };
	m_pTransformCom->Rotation_XYZ(Rot);
	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { -4.f, 1.f, 25.5f };
		m_OriginPos = { -4.f, 1.f, 1.5f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { -4.f, 1.f, 24.f };
		m_OriginPos = { -4.f, 1.f, 0.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { -4.f, 1.f, 22.5f };
		m_OriginPos = { -4.f, 1.f, -1.5f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}
	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		break;
	case 2:
		FloorAdjustPos.y += 1.5f;
		m_OriginPos.y += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 3:
		FloorAdjustPos.y += 3.f;
		m_OriginPos.y += 3.f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}

}

void CSans_Gaster::Set_ArgRightPos(SansGasterPos Pos)
{
	_float3 Rot = { 0, 90, 0 };
	m_pTransformCom->Rotation_XYZ(Rot);
	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { 4.f, 1.f, 25.5f };
		m_OriginPos = { 4.f, 1.f, 1.5f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { 4.f, 1.f, 24.f };
		m_OriginPos = { 4.f, 1.f, 0.f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { 4.f, 1.f, 22.5 };
		m_OriginPos = { 4.f, 1.f, -1.5f };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}
	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		break;
	case 2:
		FloorAdjustPos.y += 1.5f;
		m_OriginPos.y += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 3:
		FloorAdjustPos.y += 3.f;
		m_OriginPos.y += 3.f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}
}

void CSans_Gaster::Set_ArgDownPos(SansGasterPos Pos)
{
	_float3 Rot = { 90, 0, 0 };
	m_pTransformCom->Rotation_XYZ(Rot);
	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { -1.5f, -4.f, 24.f };
		m_OriginPos = { -1.5f, -4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { 0.f, -4.f, 24.f };
		m_OriginPos = { 0.f, -4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { 1.5f, -4.f, 24.f };
		m_OriginPos = { 1.5f, -4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}
	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		FloorAdjustPos.z -= 1.5f;
		m_OriginPos.z -= 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 2:

		break;
	case 3:
		FloorAdjustPos.z += 1.5f;
		m_OriginPos.z += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}
}

void CSans_Gaster::Set_ArgUpPos(SansGasterPos Pos)
{
	_float3 Rot = { 270, 0, 0 };
	m_pTransformCom->Rotation_XYZ(Rot);
	_float3 FirstPos = { 0.f, 0.f, 0.f };
	switch (Pos)
	{
	case SansGasterPos::left:
		FirstPos = { -1.5f, 4.f, 24.f };
		m_OriginPos = { -1.5f, 4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Middle:
		FirstPos = { 0, 4.f, 24.f };
		m_OriginPos = { 0, 4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::Right:
		FirstPos = { 1.5f, 4.f, 24.f };
		m_OriginPos = { 1.5f, 4.f, 0 };
		m_pTransformCom->Set_Position(FirstPos);
		break;
	case SansGasterPos::End:
		break;
	default:
		break;
	}
	_float3 FloorAdjustPos = m_pTransformCom->Get_Pos();
	switch (m_iFloor)
	{
	case 1:
		FloorAdjustPos.z -= 1.5f;
		m_OriginPos.z -= 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	case 2:
		break;
	case 3:
		FloorAdjustPos.z += 1.5f;
		m_OriginPos.z += 1.5f;
		m_pTransformCom->Set_Position(FloorAdjustPos);
		break;
	default:
		break;
	}
}

CSans_Gaster* CSans_Gaster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSans_Gaster* pInstance = new CSans_Gaster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CSans_Gaster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSans_Gaster::Clone(void* pArg)
{
	CSans_Gaster* pInstance = new CSans_Gaster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create clone: CSans_Gaster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSans_Gaster::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
