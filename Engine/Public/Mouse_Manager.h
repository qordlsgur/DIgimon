#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CMouse_Manager final : public CBase
{
private:
	CMouse_Manager();
	virtual ~CMouse_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	POINT& Get_Mouse() { return m_pCursor; }

private:
	POINT				m_pCursor{};
	ENGINE_DESC			pDesc{};
public:
	static CMouse_Manager* Create();
	virtual void Free();
};

NS_END