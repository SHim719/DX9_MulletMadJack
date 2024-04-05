#include "Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent { pGraphic_Device }
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent { rhs }
    , m_Textures { rhs.m_Textures }
    , m_iMaxTextureNum{ rhs.m_iMaxTextureNum }
{
    for (auto& pTexture : m_Textures)
        Safe_AddRef(pTexture);
}

HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
    if (iTextureIndex >= (_uint)m_Textures.size())
        return E_FAIL;

    return m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);
}

HRESULT CTexture::Initialize_Prototype(TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures)
{
    m_Textures.reserve(iNumTextures);

    for (_uint i = 0; i < iNumTextures; ++i)
    {
        _tchar  szFilePath[MAX_PATH] = TEXT("");

        wsprintf(szFilePath, strTextureFilePath.c_str(), i);

        LPDIRECT3DBASETEXTURE9			pTexture = { nullptr };

        HRESULT hr = S_OK;
        if (eTextureType == TYPE_TEXTURE2D)
            hr = D3DXCreateTextureFromFileEx(m_pGraphic_Device, szFilePath
                , D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
                D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED
                , D3DX_FILTER_NONE, D3DX_FILTER_NONE,
                0, nullptr, NULL, (LPDIRECT3DTEXTURE9*)&pTexture);
        else
            hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFilePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
      
        if (FAILED(hr))
            return E_FAIL;

        m_Textures.push_back(pTexture);

        m_iMaxTextureNum = i;   // 텍스처 번호 최대값을 구하기 위해 i 대입
    }

    return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
    return S_OK;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures)
{
    CTexture* pInstance = new CTexture(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(eTextureType, strTextureFilePath, iNumTextures)))
    {
        MSG_BOX(TEXT("Failed To Created : CTexture"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
    CTexture* pInstance = new CTexture(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CTexture"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTexture::Free()
{
    __super::Free();

    for (auto& pTexture : m_Textures)
        Safe_Release(pTexture);

    m_Textures.clear();
}
