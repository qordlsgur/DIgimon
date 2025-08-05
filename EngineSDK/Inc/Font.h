#pragma once

#include "Component.h"

NS_BEGIN(Engine)


class ENGINE_DLL CFont final : public CComponent
{
public:
	typedef struct tagFontDesc
	{
		_uint		Height{};		// 글자 높이
		_uint		Weight{};		// 글자 굵기 타입 재정의 되어있음 0~9
		LPCWSTR		Write{};		// 출력 할 글
	}FONT_DESC;

private:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(const CFont& Prototype);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Font_Setting(_uint Height, LPCWSTR Write, _float fX, _float fY, _float OriginX, _float OriginY, _uint Bold= 700);
	HRESULT Font_Setting(_uint Height, _float Write, _float fX, _float fY, _float OriginX, _float OriginY);
	HRESULT Font_Setting(_uint Height, _int Write, _float fX, _float fY, _float OriginX, _float OriginY);
	HRESULT Font_NPCSetting(_uint Height, vector<LPCWSTR> Text, _float fX, _float fY, _float OriginX, _float OriginY);
	void Font_Move(_float fX, _float fY);

	void Set_Font(LPCWSTR Write);
	void Set_Font(_float Write);
	void Set_Font(_int Write);
	void Set_Count(_int Count);

	HRESULT Render();
	HRESULT Render(_uint Alpha);
	HRESULT TextRender();
	void	Set_TextCount() { m_iCount = 0; }
	void	Text_Next() { ++m_iCount; }

private:
	ID3DXFont* m_pFont = { nullptr };
	ID3DXSprite* m_pTextSprite = { nullptr };

	_uint			m_iHeight{};
	LPCWSTR			m_strWrite{};
	_wstring		wstr{};
	_float			m_fWrite{};
	_int			m_iWrite{};
	RECT			rcPos{};
	_long			m_lLeft{}, m_lTop{};

	_float			m_fOriginX{}, m_fOriginY{};
	_float			m_fOffsetX{}, m_fOffsetY{};

	_uint			m_iCount{};
	vector<LPCWSTR>	m_pText;
	                               
public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CFont* Clone(void* pArg);
	virtual void Free() override;
};

NS_END