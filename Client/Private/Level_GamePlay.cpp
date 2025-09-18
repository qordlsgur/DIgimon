#include "Level_GamePlay.h"
#include "GameInstance.h"

#include "Camera_Free.h"
#include "Camera_Manager.h"
#include "Digimon_Manager.h"
#include "Inventory_Manager.h"
#include "DigiDex_Manager.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
	: CLevel{ pDevice, pContext, ENUM_CLASS(eLevelID) }
	, m_pDigiDex_Manager{ CDigiDex_Manager::GetInstance() }
	, m_pInventort_Manager{ CInventory_Manager::GetInstance() }
	, m_pCamera_Manager{ CCamera_Manager::GetInstance() }
	, m_pDigimon_Manager{ CDigimon_Manager::GetInstance() }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pCamera_Manager->Initialize();
	m_pDigimon_Manager->Initialize();
	m_pDigiDex_Manager->Initialize();


	//if (FAILED(Ready_Lights()))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Digimon(TEXT("Layer_Digimon"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;	

	//if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_DigiDex(TEXT("Layer_DigiDex"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Digivice(TEXT("Layer_Digivice"))))
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_UHD(TEXT("Layer_UHD"))))
		return E_FAIL;*/



	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨이빈다"));
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Inventory(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inventory"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_DigiDex(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DigiDex"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Digivice(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UHD(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PartyUHD"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraDesc{};
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1500.f;
	CameraDesc.vEye = _float3(0.f, 130.f, -50.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 30.f);
	CameraDesc.fSpeedPerSec = 5.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Digimon(const _wstring& strLayerTag)
{

	//엔젤우몬
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Angewomon"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 1; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	/*for (size_t i = 0; i < 50; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}*/

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext, eLevelID);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pDigiDex_Manager);
	Safe_Release(m_pInventort_Manager);
	Safe_Release(m_pDigimon_Manager);
}
