#include "MainApp.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "Camera_Free.h"
Client::CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{


	Safe_AddRef(m_pGameInstance);
}

HRESULT Client::CMainApp::Initialize()
{
	ENGINE_DESC				EngineDesc{};
	EngineDesc.hInstance = g_hInstance;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWindowMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Prototypes()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;


	return S_OK;
}

void Client::CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT Client::CMainApp::Render()
{
	_float4			vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	m_pGameInstance->Render_Begin(&vClearColor);

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{

	if (FAILED(m_pGameInstance->Add_Font(TEXT("1"), TEXT("../Bin/Resources/Fonts/154.1ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("2"), TEXT("../Bin/Resources/Fonts/154.2ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("3"), TEXT("../Bin/Resources/Fonts/154.3ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("4"), TEXT("../Bin/Resources/Fonts/154.4ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("5"), TEXT("../Bin/Resources/Fonts/154.5ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("6"), TEXT("../Bin/Resources/Fonts/154.6ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("7"), TEXT("../Bin/Resources/Fonts/154.7ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("8"), TEXT("../Bin/Resources/Fonts/154.8ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("9"), TEXT("../Bin/Resources/Fonts/154.9ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("10"), TEXT("../Bin/Resources/Fonts/154.10ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("11"), TEXT("../Bin/Resources/Fonts/154.11ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("12"), TEXT("../Bin/Resources/Fonts/154.12ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("13"), TEXT("../Bin/Resources/Fonts/154.13ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("14"), TEXT("../Bin/Resources/Fonts/154.14ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("15"), TEXT("../Bin/Resources/Fonts/154.15ex_Fast.spritefont"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("18"), TEXT("../Bin/Resources/Fonts/154.18ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("24"), TEXT("../Bin/Resources/Fonts/154.24ex_Fast.spritefont"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("30"), TEXT("../Bin/Resources/Fonts/154.30ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("36"), TEXT("../Bin/Resources/Fonts/154.36ex_Fast.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("42"), TEXT("../Bin/Resources/Fonts/154.42ex_Fast.spritefont"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototypes()
{
	///* For.Prototype_Component_Transform*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
	//	CTransform::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

CMainApp* Client::CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);


}
