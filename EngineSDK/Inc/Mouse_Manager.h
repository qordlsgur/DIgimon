#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CMouse_Manager final : public CBase
{
private:
	CMouse_Manager();
	virtual ~CMouse_Manager() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	POINT& Get_Mouse() { return m_pCursor; }

private:
	POINT				m_pCursor{};

	HWND				m_phWnd{};

	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CMouse_Manager* Create(HWND hWnd);
	virtual void Free();
};

NS_END