#include "..\Public\Level.h"

#include "GameInstance.h"
#include "GameObject.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

HRESULT CLevel::Load_MapObject(const wstring& strFilePath)
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

	DWORD dwByte(0);

	int iReadFlag = 0;

	while (true)
	{
		_uint iVecSize = 0;
		iReadFlag = ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		_float4x4 worldMatrix = {};
		_uint iTextureIndex = 0;
		_uint iLayerStrLength = 0;
		_tchar szLayer[MAX_PATH] = {};
		_uint iPrototypeTagLength = 0;
		_tchar szPrototypeTag[MAX_PATH] = {};

		iReadFlag += (int)ReadFile(hFile, &worldMatrix, sizeof(_float4x4), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iTextureIndex, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iLayerStrLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szLayer, sizeof(_tchar) * iLayerStrLength, &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, &iPrototypeTagLength, sizeof(_uint), &dwByte, nullptr);
		iReadFlag += (int)ReadFile(hFile, szPrototypeTag, sizeof(_tchar) * iPrototypeTagLength, &dwByte, nullptr);

		if (iReadFlag != 7)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}
			

		auto pObj = CGameInstance::Get_Instance()->Add_Clone(m_iLevelID, szLayer, szPrototypeTag);
		pObj->Set_Texture_Index(iTextureIndex);
		pObj->Get_Transform()->Set_WorldMatrix(worldMatrix);
	}

	CloseHandle(hFile);
	return S_OK;
}

void CLevel::PriorityTick(_float fTimeDelta)
{
}

void CLevel::Tick(_float fTimeDelta)
{
}

void CLevel::LateTick(_float fTimeDelta)
{
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
