#include "MapLoader.h"

#include "GameInstance.h"

#include "Door.h"
#include "SodaMachine.h"

#include "Monster_Headers.h"
#include "Trigger_Headers.h"



IMPLEMENT_SINGLETON(CMapLoader)

CMapLoader::CMapLoader()
	: m_pGameInstance { CGameInstance::Get_Instance()}
{
}

HRESULT CMapLoader::Load(const wstring& strFilePath, LEVEL eLevel)
{
	HANDLE		hFile = CreateFile(strFilePath.c_str(), // 파일 경로와 이름을 명시
		GENERIC_READ, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		OPEN_EXISTING,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}

	if (FAILED(Load_Monster(hFile, eLevel)))
	{
		assert(false);
		return E_FAIL;
	}
	if (FAILED(Load_Trigger(hFile, eLevel)))
	{
		assert(false);
		return E_FAIL;
	}
	if (FAILED(Load_MapObject(hFile, eLevel)))
	{
		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMapLoader::Load_Monster(HANDLE hFile, LEVEL eLevel)
{
	DWORD dwByte(0);

	_uint iVecSize = 0;
	int iReadFlag = (int)ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, nullptr);
	if (FALSE == iReadFlag)
		return E_FAIL;

	CSpawnTrigger::Clear();
	CSpawnTrigger::reserve(iVecSize);
	for (_uint j = 0; j < iVecSize; ++j)
	{
		_float4x4 worldMatrix = {};
		_uint iTextureIndex = 0;
		_uint iLayerStrLength = 0;
		_tchar szLayer[MAX_PATH] = {};
		_uint iPrototypeTagLength = 0;
		_tchar szPrototypeTag[MAX_PATH] = {};
		_float3 vColliderOffset;
		_float3 vColliderScale;

		iReadFlag += (int)ReadFile(hFile, &worldMatrix, sizeof(_float4x4), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iTextureIndex, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iLayerStrLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szLayer, sizeof(_tchar) * iLayerStrLength, &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iPrototypeTagLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szPrototypeTag, sizeof(_tchar) * iPrototypeTagLength, &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &vColliderOffset, sizeof(_float3), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &vColliderScale, sizeof(_float3), &dwByte, nullptr);

		CGameObject* pMonster = nullptr;
		switch (MONSTERTYPE(iTextureIndex))
		{
		case WHITE_SUIT:
			pMonster = m_pGameInstance->Add_Clone(eLevel, szLayer, L"Prototype_White_Suit");
			static_cast<CBoxCollider*>(pMonster->Find_Component(L"Collider"))->Set_Scale({ 1.3f, 1.3f, 1.f });
			break;
		case DRONE:
			pMonster = m_pGameInstance->Add_Clone(eLevel, szLayer, L"Prototype_Drone");
			break;
		}

		_float3 vPos;
		memcpy(&vPos, &worldMatrix.m[3], sizeof(_float3));
		pMonster->Get_Transform()->Set_Position(vPos);
		pMonster->Set_Active(false);

		CSpawnTrigger::Add_EnemyObj(pMonster);
		iReadFlag = 1;
	}
	
	return S_OK;
}

HRESULT CMapLoader::Load_Trigger(HANDLE hFile, LEVEL eLevel)
{
	DWORD dwByte(0);

	_uint iVecSize = 0;
	int iReadFlag = (int)ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, nullptr);
	if (FALSE == iReadFlag)
		return E_FAIL;

	for (_uint j = 0; j < iVecSize; ++j)
	{
		_float4x4 worldMatrix = {};
		_uint iTextureIndex = 0;
		_uint iLayerStrLength = 0;
		_tchar szLayer[MAX_PATH] = {};
		_uint iPrototypeTagLength = 0;
		_tchar szPrototypeTag[MAX_PATH] = {};
		_float3 vColliderOffset;
		_float3 vColliderScale;
		
		iReadFlag += (int)ReadFile(hFile, &worldMatrix, sizeof(_float4x4), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iTextureIndex, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iLayerStrLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szLayer, sizeof(_tchar) * iLayerStrLength, &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iPrototypeTagLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szPrototypeTag, sizeof(_tchar) * iPrototypeTagLength, &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &vColliderOffset, sizeof(_float3), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &vColliderScale, sizeof(_float3), &dwByte, nullptr);
		
		//CGameObject* pTrigger = nullptr;
		//switch (TRIGGERTYPE(iTextureIndex))
		//{
		//	break;
		//}
		//pTrigger->Get_Transform()->Set_WorldMatrix(worldMatrix);
		//CBoxCollider* pCollider = dynamic_cast<CBoxCollider*>(pTrigger->Find_Component(L"Collider"));
		//pCollider->Set_Scale(vColliderScale);
		//pCollider->Set_Offset(vColliderOffset);
		//pCollider->Update_BoxCollider(worldMatrix);
		iReadFlag = 1;
	}

	return S_OK;
}

HRESULT CMapLoader::Load_MapObject(HANDLE hFile, LEVEL eLevel)
{
	DWORD dwByte(0);

	int iReadFlag = 0;

	for (_uint i = WALL; i < (_uint)OBJTYPE_END; ++i)
	{
		_uint iVecSize = 0;
		int iReadFlag = (int)ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, nullptr);

		for (_uint j = 0; j < iVecSize; ++j)
		{
			_float4x4 worldMatrix = {};
			_uint iTextureIndex = 0;
			_uint iLayerStrLength = 0;
			_tchar szLayer[MAX_PATH] = {};
			_uint iPrototypeTagLength = 0;
			_tchar szPrototypeTag[MAX_PATH] = {};
			_float3 vColliderOffset;
			_float3 vColliderScale;

			iReadFlag += (int)ReadFile(hFile, &worldMatrix, sizeof(_float4x4), &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, &iTextureIndex, sizeof(_uint), &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, &iLayerStrLength, sizeof(_uint), &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, szLayer, sizeof(_tchar) * iLayerStrLength, &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, &iPrototypeTagLength, sizeof(_uint), &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, szPrototypeTag, sizeof(_tchar) * iPrototypeTagLength, &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, &vColliderOffset, sizeof(_float3), &dwByte, nullptr);
			iReadFlag += (int)ReadFile(hFile, &vColliderScale, sizeof(_float3), &dwByte, nullptr);

			auto pObj = m_pGameInstance->Add_Clone(eLevel, szLayer, szPrototypeTag);
			pObj->Set_Texture_Index(iTextureIndex);
			pObj->Get_Transform()->Set_WorldMatrix(worldMatrix);

			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(pObj->Find_Component(L"Collider"));
			if (pBoxCollider)
			{
				pBoxCollider->Set_Scale(vColliderScale);
				pBoxCollider->Set_Offset(vColliderOffset);
				pBoxCollider->Update_BoxCollider(worldMatrix);
			}

			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pObj->Find_Component(L"VIBuffer"));
			if (pVIBuffer)
				pVIBuffer->Scaling_Texcoord(pObj->Get_Transform()->Get_Scale());

			iReadFlag = 1;
		}
	}

	CLayer* pDoorLayer = m_pGameInstance->Find_Layer(eLevel, L"Door");
	auto Doors = pDoorLayer->Get_GameObjects();

	for (auto& pObj : Doors)
	{
		CDoor* pDoor = static_cast<CDoor*>(pObj);
		_bool isDoorRight;
		ReadFile(hFile, &isDoorRight, sizeof(_bool), &dwByte, nullptr);

		if (isDoorRight)
			pDoor->Set_Direction(CDoor::DIRECTION::RIGHT);
		else
			pDoor->Set_Direction(CDoor::DIRECTION::LEFT);
	}

	CLayer* pSodaMachineLayer = m_pGameInstance->Find_Layer(eLevel, L"SodaMachine");
	auto Machines = pSodaMachineLayer->Get_GameObjects();

	for (auto& pObj : Machines)
	{
		_float3 vPourPos = { 0.f, 0.f, 0.f };
		ReadFile(hFile, &vPourPos, sizeof(_float3), &dwByte, nullptr);
		static_cast<CSodaMachine*>(pObj)->Set_PourPos(vPourPos);
	}

	CLayer* pSpawnTriggerLayer = m_pGameInstance->Find_Layer(eLevel, L"SpawnTrigger");
	auto spawnTriggers = pSpawnTriggerLayer->Get_GameObjects();

	for (auto& pObj : spawnTriggers)
	{
		CSpawnTrigger* pSpawnTrigger = static_cast<CSpawnTrigger*>(pObj);
		_uint iMin, iMax;
		ReadFile(hFile, &iMin, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iMax, sizeof(_uint), &dwByte, nullptr);
		pSpawnTrigger->Set_MinIdx(iMin);
		pSpawnTrigger->Set_MaxIdx(iMax);
	}

	CloseHandle(hFile);
	return S_OK;
}

void CMapLoader::Free()
{
	Destroy_Instance();
}
