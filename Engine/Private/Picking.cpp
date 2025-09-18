#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice },
    m_pContext{ pContext },
    m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    D3D11_VIEWPORT       ViewportDesc{};
    _uint                iNumViewports = { 1 };

    m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


    m_fWinSizeX = ViewportDesc.Width;
    m_fWinSizeY = ViewportDesc.Height;

    return S_OK;
}

void CPicking::Update()
{
    /* 월드상에서의 마우스의 정보를 구한다 .*/

   /* 뷰포트 상의 마우스 좌표를 구한다. */
    POINT       ptMouse = {};
    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);

    _float4         vMousePos{};

    /* 투영스페이스 상의 마우스 좌표를 구한다. (0, 0) -> (-1, 1) (wsx,wsy) -> (1, -1) */
    vMousePos.x = ptMouse.x / (m_fWinSizeX * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(m_fWinSizeY * 0.5f) + 1.f;
    vMousePos.z = 0.f; /* near를 클릭한 것이기 때문에 .*/
    vMousePos.w = 1.f; /* w로 나눠서 결정된 값이기 대문에. */

    _vector vMouse_NDC = XMLoadFloat4(&vMousePos);

    /* 뷰스페이스 상의 마우스 좌표를 구한다. */
    vMouse_NDC = XMVector4Transform(vMouse_NDC, XMMatrixInverse(nullptr, m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ)));
    vMouse_NDC = XMVectorDivide(vMouse_NDC, XMVectorSplatW(vMouse_NDC));

    _float3         RayPos{}, RayDir{};
    RayPos = _float3(0.f, 0.f, 0.f);
    XMStoreFloat3(&RayDir, vMouse_NDC);

    _matrix View = XMMatrixInverse(nullptr, m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));

    /* 월드스페이스 상의 마우스 좌표를 구한다. */
    XMStoreFloat3(&m_vRayPos[ENUM_CLASS(RAY::WORLD)], XMVector3TransformCoord(XMLoadFloat3(&RayPos), View));
    XMStoreFloat3(&m_vRayDir[ENUM_CLASS(RAY::WORLD)], (XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&RayDir), View))));
}

void CPicking::Transform_ToLocalSpace(const _matrix* pWorldMatrixInverse)
{
    XMStoreFloat3(&m_vRayPos[ENUM_CLASS(RAY::LOCAL)], XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos[ENUM_CLASS(RAY::WORLD)]), *pWorldMatrixInverse));
    XMStoreFloat3(&m_vRayDir[ENUM_CLASS(RAY::LOCAL)], XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::WORLD)]), *pWorldMatrixInverse));

    XMStoreFloat3(&m_vRayDir[ENUM_CLASS(RAY::LOCAL)], (XMVector3Normalize(XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::LOCAL)]))));
}

_bool CPicking::Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fDist{};

    _bool       isColl = TriangleTests::Intersects(XMLoadFloat3(&m_vRayPos[ENUM_CLASS(RAY::WORLD)]), XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::WORLD)]), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

    if (true == isColl)
    {
        XMStoreFloat3(pOut, XMLoadFloat3(&m_vRayPos[ENUM_CLASS(RAY::WORLD)]) + XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::WORLD)]) * fDist);
    }

    return isColl;
}

_bool CPicking::Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fU{}, fV{}, fDist{};

    _bool       isColl = TriangleTests::Intersects(XMLoadFloat3(&m_vRayPos[ENUM_CLASS(RAY::LOCAL)]), XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::LOCAL)]), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);


    if (true == isColl)
    {
        XMStoreFloat3(pOut, XMLoadFloat3(&m_vRayPos[ENUM_CLASS(RAY::LOCAL)]) + XMLoadFloat3(&m_vRayDir[ENUM_CLASS(RAY::LOCAL)]) * fDist);
    }

    return isColl;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
    CPicking* pInstance = new CPicking(pDevice, pContext);

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        MSG_BOX("Failed to Created : CPicking");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CPicking::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
