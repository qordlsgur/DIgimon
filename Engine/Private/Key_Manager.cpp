#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
    ZeroMemory(m_byPrevious_KeyStates, sizeof(m_byPrevious_KeyStates));
    ZeroMemory(m_byCurrent_KeyStates, sizeof(m_byCurrent_KeyStates));        
}

HRESULT CKey_Manager::Initialize(HINSTANCE hInst, HWND hWnd)
{
    // DInput 컴객체를 생성하는 함수
    if (FAILED(DirectInput8Create(hInst,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pDInput,
        NULL)))
        return E_FAIL;

    // 키보드 객체 생성
    if (FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
        return E_FAIL;

    // 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
    m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);

    // 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
    m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // 장치에 대한 access 버전을 받아오는 함수
    m_pKeyboard->Acquire();


    // 마우스 객체 생성
    if (FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;

    // 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
    m_pMouse->SetDataFormat(&c_dfDIMouse);

    // 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // 장치에 대한 access 버전을 받아오는 함수
    m_pMouse->Acquire();

    return S_OK;
}

void CKey_Manager::Update(_float fTimeDelta)
{
    // 이전 마우스 상태 저장
    m_byPrevious_MouseStates = m_byCurrent_MouseStates;

    // 이전 키 상태 저장
    memcpy(m_byPrevious_KeyStates, m_byCurrent_KeyStates, sizeof(m_byCurrent_KeyStates));

    // 마우스 현재 상태 읽기
    m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_byCurrent_MouseStates);


    // 키보드 현재 상태 읽기
    m_pKeyboard->GetDeviceState(256, m_byCurrent_KeyStates);

}

_bool CKey_Manager::Key_Down(_byte Key)
{
    return (m_byCurrent_KeyStates[Key] & 0x80) && !(m_byPrevious_KeyStates[Key] & 0x80);
}

_bool CKey_Manager::Key_Up(_byte Key)
{
    return !(m_byCurrent_KeyStates[Key] & 0x80) && (m_byPrevious_KeyStates[Key] & 0x80);
}

_bool CKey_Manager::Key_Pressing(_byte Key)
{
    return (m_byCurrent_KeyStates[Key] & 0x80);
}

_bool CKey_Manager::Mouse_Down(MOUSEKEYSTATE eState)
{
    return (m_byCurrent_MouseStates.rgbButtons[static_cast<_uint>(eState)] & 0x80) && !(m_byPrevious_MouseStates.rgbButtons[static_cast<_uint>(eState)] & 0x80);
}

_bool CKey_Manager::Mouse_Up(MOUSEKEYSTATE eState)
{
    return !(m_byCurrent_MouseStates.rgbButtons[static_cast<_uint>(eState)] & 0x80) && (m_byPrevious_MouseStates.rgbButtons[static_cast<_uint>(eState)] & 0x80);
}

_bool CKey_Manager::Mouse_Pressing(MOUSEKEYSTATE eState)
{
    return (m_byCurrent_MouseStates.rgbButtons[static_cast<_uint>(eState)] & 0x80);
}

_long CKey_Manager::Mouse_Drag(MOUSEMOVESTATE eMouseState)
{
    return *((reinterpret_cast<_int*>(&m_byCurrent_MouseStates)) + static_cast<_uint>(eMouseState));
}

CKey_Manager* CKey_Manager::Create(HINSTANCE hInstance, HWND hWnd)
{
    CKey_Manager* pInstance = new CKey_Manager();

    if (FAILED(pInstance->Initialize(hInstance, hWnd)))
    {
        MSG_BOX("Failed to Created : CKey_Manager");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CKey_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pKeyboard);
    Safe_Release(m_pMouse);
}                