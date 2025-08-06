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
	bool Key_Down(_ubyte Key);
	bool Key_Up(_ubyte Key);
	bool Key_Pressing(_ubyte Key);

	bool Mouse_Down(_ubyte Key);
	bool Mouse_Up(_ubyte Key);
	bool Mouse_Pressing(_ubyte Key);

private:
	   LPDIRECTINPUT8			m_pDInput = nullptr;
	   LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	   LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

	   // 키보드이 갯수256개
	   _ubyte						m_byPrevious_KeyStates[DIK_KEY] = {};			// 이전 키보드 상태
	   _ubyte						m_byCurrent_KeyStates[DIK_KEY] = {};			// 현재 키보드 상태

	   // 마우스는 3개 0은 좌클릭, 1은 우클릭, 2는 휠
	   _ubyte						m_byPrevious_MouseStates[DIK_MOUSE] = {};		// 이전 마우스 상태
	   _ubyte						m_byCurrent_MouseStates[DIK_MOUSE] = {};		// 현재 마우스 상태
	   
public:
	static CKey_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

NS_END