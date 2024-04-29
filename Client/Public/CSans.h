#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "Animation.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
END

BEGIN(Client)

enum class SANSSTATE
{
	STATE_IDLE,
	STATE_PATTERN1,
	STATE_PATTERN2,
	STATE_PATTERN3,
	STATE_PATTERN4,
	STATE_PATTERN5,
	STATE_PATTERN6,
	STATE_PATTERN7,
	STATE_PATTERN8,
	STATE_PATTERN9,
	STATE_PATTERN10,
	STATE_PATTERN11,
	STATE_PATTERN12,
	STATE_PATTERN13,
	STATE_PATTERN14,
	STATE_PATTERN15,
	STATE_PATTERN16,
	STATE_PATTERN17,
	STATE_PATTERN18,
	STATE_PATTERN19,
	STATE_PATTERN20,
	STATE_PATTERN21,
	STATE_PATTERN22,
	STATE_PATTERN23,
	STATE_PATTERN24,
	STATE_DEATH,
	STATE_END
};
enum class SansTurnBased
{
	SansTurn,
	PlayerTurn,
	SansText,
	End,
};
class CSans : public CPawn
{
private:

	enum class PatternTimeGAP
	{
		SMALL,
		DEFAULT,
		BIG,
	};
private:
	CSans(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSans(const CSans& rhs);
	virtual ~CSans() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void RenderBegin();
	virtual HRESULT Render() override;
	void RenderBody();
	void RenderEnd();

public:
	void SetTurn(SansTurnBased Turn);
	SansTurnBased GetSansTurnInfo() { return m_Turn; }
	_uint GetPatternCount() { return m_iPatternCount; }
	void TransParent();

	void Set_Measured_Sans_Lines(_bool bMeasured) { m_bMeasured_Sans_Lines = bMeasured; }
	_bool Get_Measured_Sans_Lines() { return m_bMeasured_Sans_Lines; }

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();


private:
	void SubPatternTime(_float fTimeDelta);
	void Set_Pattern0Time();
	void Set_Pattern1Time();
	void Set_Pattern2Time();
	void Set_Pattern3Time();
	void Set_Pattern4Time();
	void Set_Pattern5Time();
	void Set_Pattern14Time(); // 构 老捞 任纠 浆况瘤摆瘤
	
	void Set_Pattern17Time(); //6
	void Set_Pattern18Time(); //3

	void Set_PatternDefaultTime();

private:
	void Add_SansPattern();
	void State_Pattern();
	

private:
	void Set_Text();
	void Set_TextLength(_float fTimeDelta);
	void Adjust_BodyPos(_float fTimeDelta);

private:
	_float m_fBodyPosTime = { 0 };
	_uint m_iSansIndex = { 0 };
	_uint m_iPatternCount = { 0 };
	_float m_fPatternTime = { 0 };
	_float m_fTextLengthTime = { 0.f };
	_float m_fLineStartTime = { 0.f };
	_int m_iDetailPatternCount = { 0 };
	_int m_iSansTextCount = { -1 };
	_uint m_iSansTextLength = { 0 };
	_float m_fTransParentTime = { 0 };
	SansTurnBased m_Turn = { SansTurnBased::SansTurn };
	

	// delete this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	_uint m_itestCount = { 0 };
	_bool m_bBGMPlaying = { false };
	_bool m_bSansSpeaking = { false };
	_bool m_bMeasured_Sans_Lines = { false };
	_float m_fSans_Words_Timelength = { 0 };

private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTexture* m_pBodyTextureCom = { nullptr };
	CVIBuffer* m_pVIBodyBufferCom = { nullptr };
	CTransform* m_pBodyTransformCom = {nullptr};
	CTexture* m_pLegTextureCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };
	class CBone_Spawner* m_pBoneSpawner = { nullptr };
	class CGaster_Spawner* m_pGasterSpawner = { nullptr };
	CTransform* m_pPlayerTransformCom = { nullptr };


public:
	static CSans* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END