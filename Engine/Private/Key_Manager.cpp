#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
}

HRESULT CKey_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    ENGINE_DESC pDesc;
    
    // Direct Input 인터페이스를 초기화 한다.
    if(FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, nullptr)))
        return E_FAIL;

    // 키보드 디바이스 생성
    // 키보드의 Direct Input 인터페이스를 초기화 한다.
    if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
        return E_FAIL;

    // 데이터 형식을 설정 하는데 이 경우 키보드 이므로 사전 정의된 데이터 형식을 사용할 수 있음
    if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
        return E_FAIL;

    // 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정한다.
    if (FAILED(m_pKeyboard->SetCooperativeLevel(pDesc.hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
        return E_FAIL;

    // 키보드를 할당 받는다.
    if (FAILED(m_pKeyboard->Acquire()))
        return E_FAIL;

    // 마우스 디바이스 생성
    // 마우스의 Direct Input 인터페이스를 초기화 한다.
    if (FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;

    // 데이터 형식을 설정 하는데 이 경우 키보드 이므로 사전 정의된 데이터 형식을 사용할 수 있음
    if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
        return E_FAIL;

    // 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정한다.
    if (FAILED(m_pMouse->SetCooperativeLevel(pDesc.hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
        return E_FAIL;

    // 키보드를 할당 받는다.
    if (FAILED(m_pMouse->Acquire()))
        return E_FAIL;

    return S_OK;
}

void CKey_Manager::Update(_float fTimeDelta)
{
	HRESULT hr;

    // 이전 마우스 상태 저장
    memcpy(m_byPrevious_MouseStates, m_byCurrent_MouseStates, sizeof(m_byCurrent_MouseStates));

    // 이전 키 상태 저장
    memcpy(m_byPrevious_KeyStates, m_byCurrent_KeyStates, sizeof(m_byCurrent_KeyStates));

    // 마우스 현재 상태 읽기
    hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_byCurrent_MouseStates);
    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
            m_pMouse->Acquire();
    }

    // 키보드 현재 상태 읽기
     hr = m_pKeyboard->GetDeviceState(sizeof(m_byCurrent_KeyStates), m_byCurrent_KeyStates);
    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
            m_pKeyboard->Acquire();
    }
}


bool CKey_Manager::Key_Down(_ubyte Key)
{
    return (m_byCurrent_KeyStates[Key] & 0x80) && !(m_byPrevious_KeyStates[Key] & 0x80);
}

bool CKey_Manager::Key_Up(_ubyte Key)
{
    return !(m_byCurrent_KeyStates[Key] & 0x80) && (m_byPrevious_KeyStates[Key] & 0x80);
}

bool CKey_Manager::Key_Pressing(_ubyte Key)
{
    return (m_byCurrent_KeyStates[Key] & 0x80);
}

bool CKey_Manager::Mouse_Down(_ubyte Key)
{
    return (m_byCurrent_MouseStates[Key] & 0x80) && !(m_byPrevious_MouseStates[Key] & 0x80);
}

bool CKey_Manager::Mouse_Up(_ubyte Key)
{
    return !(m_byCurrent_MouseStates[Key] & 0x80) && (m_byPrevious_MouseStates[Key] & 0x80);
}

bool CKey_Manager::Mouse_Pressing(_ubyte Key)
{
    return (m_byCurrent_MouseStates[Key] & 0x80);
}

CKey_Manager* CKey_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CKey_Manager* pInstance = new CKey_Manager();

    if (FAILED(pInstance->Initialize(pDevice, pContext)))
    {
        MSG_BOX("Failed to Created : CKey_Manager");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CKey_Manager::Free()
{
    __super::Free();
}