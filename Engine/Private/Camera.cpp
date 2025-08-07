#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CCamera::CCamera(const CCamera& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// Transform에서는 float4의 값인데 Eye나 At은 float3라서 w의 값을 1로 직접 던져준다.
	// 카메라의 위치를 내가 세팅한 카메라의 위치로 옮겨준다.
	m_pTransformCom->Set_State(STATE::POSITION,XMVectorSetW(XMLoadFloat3(&pDesc->vEye), 1.f));

	// 카메라가 지금 위치에서 어디를 바라볼지 정해준다.
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&pDesc->vAt), 1.f));

	// 
	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT	ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	// 카메라의 Fov랑 Near, Far 값을 다 들고온다.
	m_fFovy = pDesc->fFovy;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;
	// Aspect값을 맞추기 위해서 지금 게임 화면의 가로 세로를 설정 해준다.
	m_fAspect = ViewportDesc.Width / ViewportDesc.Height;

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

void CCamera::Update(_float fTimeDelta)
{
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Bind_Matrices()
{
	// PipeLine에 뷰행렬을 던져준다.
	// 카메라의 뷰 행렬을 하기 위해서 카메라 월드 행렬의 역행렬을 던져준다.
	// 원래 기본적인 오브젝트는 월드행렬의 역행렬은 로컬행렬인데 카메라는 뷰 행렬이 된다.
	m_pGameInstance->Set_Transform(D3DTS::VIEW, XMMatrixInverse(nullptr,	XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr())));

	// PipeLine에 투영 행렬을 만들기 위해 값을 던져준다.
	// XMMatrixPerspectiveFovLH에 값을 던져주면 만들어준다.
	m_pGameInstance->Set_Transform(D3DTS::PROJ, XMMatrixPerspectiveFovLH(m_fFovy,	m_fAspect, m_fNear, m_fFar));

	return S_OK;
}


HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
}
