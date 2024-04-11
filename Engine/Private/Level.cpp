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

HRESULT CLevel::Load_MapObject(const wstring& strFilePath, _uint iMaxObjType)
{
	HANDLE		hFile = CreateFile(strFilePath.c_str(), // ���� ��ο� �̸��� ���
		GENERIC_READ, // ���� ���� ���(GENERIC_WRITE : ���� ����, GENERIC_READ : �б� ����)
		NULL, // ���� ���, ������ ����� ���¿��� �ٸ� ���μ����� ���� ������ �ϴ� ���� �㰡�� ���ΰ�(NULL �������� ����)
		NULL, // ���� �Ӽ�, NULL �� ��� �⺻ ���� ����
		OPEN_EXISTING,	// ���� ���, (CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�б� ����, ���� ���� ���),FILE_ATTRIBUTE_NORMAL : Ư�� �Ӽ��� ���� �Ϲ� ���� ���� 
		NULL); // ������ ������ �Ӽ��� ������ ���ø� ���� ( ������� �ʱ� ������ NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}

	DWORD dwByte(0);

	int iReadFlag = 0;

	for (_uint i = 0; i <= (_uint)iMaxObjType; ++i)
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

			auto pObj = m_pGameInstance->Add_Clone(m_iLevelID, szLayer, szPrototypeTag);
			pObj->Set_Texture_Index(iTextureIndex);
			pObj->Get_Transform()->Set_WorldMatrix(worldMatrix);

			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(pObj->Find_Component(L"Collider"));
			if (pBoxCollider)
			{
				pBoxCollider->Set_Scale(vColliderScale);
				pBoxCollider->Set_Offset(vColliderOffset);
			}

			
			
			
			
			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pObj->Find_Component(L"VIBuffer"));
			pVIBuffer->Scaling_Texcoord(pObj->Get_Transform()->Get_Scale());

			iReadFlag = 1;
		}
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
