#include "Camera_Free.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pCamera_Manager = CCamera_Manager::GetInstance();
	m_pCamera_Manager->Add_Camera(this);

	m_fCameraHeightOffset = XMVectorSet(0.f, 10.f, 0.0f, 0.0f);

	m_fDefaultDistanceY = 25.f;
	m_fDefaultDistanceZ = -22.5f;
	m_fCameraDistanceOffset = XMVectorSet(0.f, m_fDefaultDistanceY, m_fDefaultDistanceZ, 0.f);

	// 40 , -50
	m_fMaxDistanceY = 100.f;
	m_fMaxDistanceZ = -100.f;
	m_fMaxCameraDistance = XMVectorSet(0.f, m_fMaxDistanceY, m_fMaxDistanceZ, 0.f);


	m_fMinDistanceY = 10.f;
	m_fMinDistanceZ = -2.f;
	m_fMinCameraDistance = XMVectorSet(0.f, m_fMinDistanceY, m_fMinDistanceZ, 0.f);

	m_fBottomCamera = XMVectorSet(0.f, -10.f, -3.f, 0.f);

	m_fLerp = 0.1f;


	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	//if (!m_bBattle)
	//{
		_long		MouseMove = {};

		if (m_pCamera_Manager->HasPlayer() == true)
		{
			m_pCamera_Manager->PlayerPos(m_fPlayerPos);
		}

		if (m_pGameInstance->Mouse_WheelUp())
		{
			m_fCameraDistanceOffset = XMVectorLerp(m_fCameraDistanceOffset, m_fMinCameraDistance, m_fLerp);
		}

		if (m_pGameInstance->Mouse_WheelDown())
		{
			m_fCameraDistanceOffset = XMVectorLerp(m_fCameraDistanceOffset, m_fMaxCameraDistance, m_fLerp);
		}



		if (m_bRightClick)
		{
			if (MouseMove = m_pGameInstance->Mouse_Drag(MOUSEMOVESTATE::X))
			{
				m_fY += fTimeDelta * MouseMove * m_fMouseSensor;
			}

			if (MouseMove = m_pGameInstance->Mouse_Drag(MOUSEMOVESTATE::Y))
			{
				//45
				m_fX += fTimeDelta * MouseMove * m_fMouseSensor;
				if (m_fX < XMConvertToRadians(-110.f))
					m_fX = XMConvertToRadians(-110.f);
				else if (m_fX > XMConvertToRadians(40.f))
					m_fX = XMConvertToRadians(40.f);

				//if (m_fX < XMConvertToRadians(0.f))
				//{
				//	if (!m_bSaveCamera)
				//	{
				//		m_fSaveCamera = XMVectorGetZ(m_fCameraDistanceOffset);
				//		m_bSaveCamera = true;
				//	}
				//	m_fCameraDistanceOffset = XMVectorLerp(m_fCameraDistanceOffset, m_fBottomCamera, fTimeDelta);
				//}
				//else
				//{
				//	if (m_bSaveCamera)
				//	{
				//		m_fCameraDistanceOffset.m128_f32[2] = m_fSaveCamera;
				//		m_bSaveCamera = false;
				//	}
				//	m_fCameraDistanceOffset = XMVectorLerp(m_fCameraDistanceOffset, m_fBottomCamera, fTimeDelta);

				//}
			}
		}


		if (m_pGameInstance->Mouse_Pressing(MOUSEKEYSTATE::RBUTTON))
			Elapsed_Time(fTimeDelta);

		if (m_pGameInstance->Mouse_Up(MOUSEKEYSTATE::RBUTTON))
		{
			m_fTime = 0.f;
			if (m_bRightClick)
			{
				ClientToScreen(g_hWnd, &m_pMousePos);
				SetCursorPos(m_pMousePos.x, m_pMousePos.y);
				ShowCursor(true);
				m_bRightClick = false;
			}
			m_pCamera_Manager->Set_LockCamera(m_bRightClick);
		}

		if (!m_bBattle)
		{
			LookPlayer(fTimeDelta);
		}
		else
			LookField(fTimeDelta);

		__super::Bind_Matrices();
	//}
}

void CCamera_Free::Update(_float fTimeDelta)
{
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{


}

HRESULT CCamera_Free::Render()
{

	return S_OK;
}

void CCamera_Free::Elapsed_Time(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime >= 0.3f)
	{
		if (!m_bRightClick)
		{
			ShowCursor(false);
			m_bRightClick = true;
			m_pCamera_Manager->Set_LockCamera(m_bRightClick);
		}
		Save_MousePos();
	}
}

void CCamera_Free::Save_MousePos()
{
	GetCursorPos(&m_pMousePos);
	ScreenToClient(g_hWnd, &m_pMousePos);

	if (m_pMousePos.x <= 0)
		m_pMousePos.x = 0;

	if (m_pMousePos.x >= g_iWinSizeX)
		m_pMousePos.x = g_iWinSizeX;

	if (m_pMousePos.y <= 0)
		m_pMousePos.y = 0;

	if (m_pMousePos.y >= g_iWinSizeY)
		m_pMousePos.y = g_iWinSizeY;

}


void CCamera_Free::LookPlayer(_float fTimeDelta)
{
	if (m_pCamera_Manager->HasPlayer() == true)
	{
		_vector vPlayerPos = m_fPlayerPos;
		_vector Offset = m_fCameraDistanceOffset;

		_matrix matRotX = XMMatrixRotationX(m_fX);
		_matrix matRotY = XMMatrixRotationY(m_fY);
		m_pCamera_Manager->Camera_Angle(m_fY);

		_vector        vQuternion = XMQuaternionRotationRollPitchYaw(m_fX, m_fY, 0.f);
		_matrix        RotationMatrix = XMMatrixRotationQuaternion(vQuternion);

		_vector vRotOffset = XMVector3TransformNormal(Offset, RotationMatrix);

		_vector vCamerapos = XMVectorAdd(vPlayerPos, vRotOffset);

		m_pTransformCom->Set_State(STATE::POSITION, vCamerapos);
		m_pTransformCom->LookAt(vPlayerPos);
	}
}

void CCamera_Free::LookField(_float fTimeDelta)
{
	_vector vPlayerPos = XMVectorSet(200.f, 0.f, 200.f, 1.f);
	_vector Offset = m_fCameraDistanceOffset;

	_matrix matRotX = XMMatrixRotationX(m_fX);
	_matrix matRotY = XMMatrixRotationY(m_fY);
	m_pCamera_Manager->Camera_Angle(m_fY);

	_vector        vQuternion = XMQuaternionRotationRollPitchYaw(m_fX, m_fY, 0.f);
	_matrix        RotationMatrix = XMMatrixRotationQuaternion(vQuternion);

	_vector vRotOffset = XMVector3TransformNormal(Offset, RotationMatrix);

	_vector vCamerapos = XMVectorAdd(vPlayerPos, vRotOffset);

	m_pTransformCom->Set_State(STATE::POSITION, vCamerapos);
	m_pTransformCom->LookAt(vPlayerPos);
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
