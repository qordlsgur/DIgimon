#include "Font.h"

CFont::CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont::Initialize(const _tchar* pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	return S_OK;
}

HRESULT CFont::Render(const _tchar* pText, const _float2& vPosition, _fvector vColor)
{
	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor);

	m_pBatch->End();

	return S_OK;
}

CFont* CFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CFont* pInstance = new CFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
