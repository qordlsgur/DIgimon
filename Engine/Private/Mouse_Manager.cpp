#include "Mouse_Manager.h"

#include "GameInstance.h"

CMouse_Manager::CMouse_Manager()
{
}

HRESULT CMouse_Manager::Initialize()
{
    return S_OK;
}

void CMouse_Manager::Priority_Update(_float fTimeDelta)
{
    GetCursorPos(&m_pCursor);
    ScreenToClient(pDesc.hWnd, &m_pCursor);
}

void CMouse_Manager::Update(_float fTimeDelta)
{
    m_pGameInstance->Mouse_Down(MOUSEKEYSTATE::LBUTTON);
}

void CMouse_Manager::Late_Update(_float fTimeDelta)
{
}

CMouse_Manager* CMouse_Manager::Create()
{
    CMouse_Manager* pInstance = new CMouse_Manager();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CMouse_Manager");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMouse_Manager::Free()
{
    __super::Free();
}
