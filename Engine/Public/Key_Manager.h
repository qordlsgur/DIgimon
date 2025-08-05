#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void Update(_float fTimeDelta);

public:
	bool Key_Down();
	bool Key_Up();
	bool Key_Pressing();

	bool Mouse_Down();
	bool Mouse_Up();
	bool Mouse_Pressing();

private:
	   LPDIRECTINPUT8			m_pDInput = nullptr;
	   LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	   LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

	   BYTE						m_byPrevious_KeyStates[DIK_KEY] = {};
	   BYTE						m_byCurrent_KeyStates[DIK_KEY] = {};
	   BYTE						m_byPrevious_MouseStates[DIK_MOUSE] = {};
	   BYTE						m_byCurrent_MouseStates[DIK_MOUSE] = {};
	   
public:
	static CKey_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

NS_END