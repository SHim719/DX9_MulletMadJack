#pragma once

#include "Base.h"
#include "Collider.h"


BEGIN(Engine)
class CCollision_Manager : public CBase
{
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

	HRESULT Initialize();

public:
	void Tick();

private:
	list<RAY_DESC>	m_RayDescs; //�÷��̾ ���̸� ��� GameInstance�� Add_RayDesc�� ȣ���ؼ� ���������� ���⿡ �־���

private:
	/*�������Ӹ��� ȣ��ż� ���� ���̿� ��ŷ������ Collider ������Ʈ�� On_Ray_Intersectȣ��
		Collider�� On_Ray_Intersect�� GameObject�� On_Ray_Intersect�� ȣ��
		On_Ray_Intersect�� �����Լ��̹Ƿ� ��ӹ޾Ƽ� ����ϸ� ��.*/
	void Intersect_Ray(); 
						  
public: 
	void Add_RayDesc(const RAY_DESC& RayDesc) { m_RayDescs.push_back(RayDesc); }

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CCollision_Manager* Create();
	void Free() override;

};

END

