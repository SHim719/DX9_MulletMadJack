#pragma once

#include "Component.h"

BEGIN(Engine)

typedef vector<LPDIRECT3DBASETEXTURE9> TEXTURES;
class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_TEXTURE2D, TYPE_CUBEMAP, TYPE_END };

private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Bind_Texture(_uint iTextureIndex);
	//POINT   Get_TextureSize(_uint iTextureIndex);
public:
	virtual HRESULT Initialize_Prototype(TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_uint		Get_MaxTextureNum() { return m_iMaxTextureNum; }
	TEXTURES	Get_Textures() { return m_Textures; }

private:
	vector<LPDIRECT3DBASETEXTURE9>	m_Textures;
	typedef vector<LPDIRECT3DBASETEXTURE9> TEXTURES;

	/*vector<POINT>					m_vTextureSizes;*/
	_uint							m_iMaxTextureNum = 0;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;



private:
	string TCharToString(const TCHAR* pSource)
	{
		_uint iLength = lstrlen(pSource) + 1;
		char* pDest = new char[iLength];
		ZeroMemory(pDest, iLength);
		WideCharToMultiByte(CP_ACP, 0, pSource, -1, pDest, iLength, nullptr, nullptr);

		string strDest = pDest;
		Safe_Delete_Array(pDest);

		return strDest;
	}
};

END