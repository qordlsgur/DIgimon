#include "Battle_Camera.h"
#include "GameInstance.h"
#include "Camera_Manager.h"

CBattle_Camera::CBattle_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CBattle_Camera::CBattle_Camera(const CBattle_Camera& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CBattle_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Camera::Initialize(void* pArg)
{

	CAMERA_BATTLE_DESC* pDesc = static_cast<CAMERA_BATTLE_DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pCamera_Manager = CCamera_Manager::GetInstance();
	m_pCamera_Manager->Add_Camera(this);

	return S_OK;
}

void CBattle_Camera::Priority_Update(_float fTimeDelta)
{
	//if (m_bBattle)
	//{
	//	ShowCursor(true);
	//__super::Bind_Matrices();
	//}
}

void CBattle_Camera::Update(_float fTimeDelta)
{
}

void CBattle_Camera::Late_Update(_float fTimeDelta)
{
}

HRESULT CBattle_Camera::Render()
{
	return S_OK;
}

void CBattle_Camera::Save_MousePos()
{
}

CBattle_Camera* CBattle_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Camera* pInstance = new CBattle_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBattle_Camera::Clone(void* pArg)
{
	CBattle_Camera* pInstance = new CBattle_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBattle_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Camera::Free()
{
	__super::Free();

}
