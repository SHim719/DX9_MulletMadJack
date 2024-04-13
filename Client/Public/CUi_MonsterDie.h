#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

enum class eMonsterGrade
{
	Low = 0,
	Middle,
	High,
	Special,
	End
};


enum class eMoveLogic
{
	Slow,
	Fast,
	Bound,
	Up,
	Random,
	End
};

class CUi_MonsterDie final : public CUi
{
public:
	typedef struct MonsterDieArg
	{
		_float MonsterDiePosX = 0;
		_float MonsterDiePosY = 0;
		_float3 MonsterRotation = { 0, 0, 0 };
		eMonsterGrade MonsterGrade;
	}MonsterDie_Arg;

private:
	CUi_MonsterDie(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_MonsterDie(const CUi_MonsterDie& rhs);
	virtual ~CUi_MonsterDie() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	
protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


protected:
	virtual void Default_Set_LifeTime() override;
	virtual void Default_Set_Size() override;
	

protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override; 
	virtual void Initialize_Set_Speed() override;


private:
	void Initialize_Set_Background();
	void Initialize_MoveLogic_Select();


private:
	void Move(_float fTimeDelta);
	void Teleport();
	void Move_Slow(_float fTimeDelta);
	void Move_Fast(_float fTimeDelta);
	void Move_Bound(_float fTimeDelta);
	void Move_Up(_float fTimeDelta);
	void Move_Random(_float fTimeDelta);
	void Adjust_YPos(_float fTimeDelta);
	void Random_Direction();
	void Random_Speed();



public:
	void Set_Pos(_float3 Position);
	void Set_Speed(_float Speed) { m_pTransformCom->Set_Speed(Speed); }
	void Scaling(_float fTimeDelta);

private:
	eMoveLogic m_eMoveLogic = { eMoveLogic::End };
	bool m_bStartPosRight = { false };
	_float m_fScaleTime = { 0.2f };
	_float m_fRandomLogicTimeGap = { 0 };
	_float3 m_fScaleDownLimit = {128, 32, 0};
	class CUi_Background* m_pBackGround = { nullptr };

public:
	static CUi_MonsterDie* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg) override; //pArg -> MonsterDie_Arg
	virtual void Free() override;
};

END
