#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWmd);

	void Update(_float fTimeDelta);

public:
	_bool Key_Down(_byte Key);
	_bool Key_Up(_byte Key);
	_bool Key_Pressing(_byte Key);
	
	_bool Mouse_Down(MOUSEKEYSTATE eState);
	_bool Mouse_Up(MOUSEKEYSTATE eState);
	_bool Mouse_Pressing(MOUSEKEYSTATE eState);

	_long Mouse_Drag(MOUSEMOVESTATE eMouseState);
private:
	   LPDIRECTINPUT8			m_pDInput = nullptr;

	   LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	   LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

	   // 키보드이 갯수256개
	   _byte						m_byPrevious_KeyStates[256] = {};			// 이전 키보드 상태
	   _byte						m_byCurrent_KeyStates[256] = {};			// 현재 키보드 상태

	   // 마우스는 3개 0은 좌클릭, 1은 우클릭, 2는 휠
	   DIMOUSESTATE					m_byPrevious_MouseStates = {};		// 이전 마우스 상태
	   DIMOUSESTATE					m_byCurrent_MouseStates = {};		// 현재 마우스 상태
	   
public:
	static CKey_Manager* Create(HINSTANCE hInstance, HWND hWnd);
	virtual void Free();
};

NS_END