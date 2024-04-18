#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Core_Camera.h"
#include "Level_GamePlay.h"
#include "Level_Manager.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CFPS_Camera final : public CCoreCamera
{
private:
	CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFPS_Camera(const CFPS_Camera& rhs);
	virtual ~CFPS_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	_float m_fVerticalAngle = 0.f;
	_float m_fVerticalAngleLimit = 40.f;

private:
	_float4x4			m_ViewMatrix;
	_float4x4			m_ProjMatrix;
	_float4x4			m_BillboardMatrix;

public:
	_float4x4		Get_Billboard_Matrix() { return m_BillboardMatrix; }
	CTransform* Get_Camera_TransformCom() { return m_pTransformCom; }

	//delete this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	_float4x4		Get_Proj_Matrix() const { return m_ProjMatrix; }
	_float4x4		Get_View_Matrix() const { return m_ViewMatrix; }
private:
	void Mouse_Ray();
	


protected:
	virtual HRESULT Add_Components() override;
	virtual void	Key_Input(_float fTimeDelta) override;
	//virtual void Update_Matrix() {};
	//virtual void Update_Info() {};

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END